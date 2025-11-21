#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "tasks/task_mqtt.h"
#include "config.h"
#include "tracking_latas.h"
#include "estado_comum.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>

#ifndef MQTT_PUBLISH_QUEUE_LENGTH
#define MQTT_PUBLISH_QUEUE_LENGTH 32
#endif

#ifndef MQTT_PUBLISH_TOPIC_MAX_LEN
#define MQTT_PUBLISH_TOPIC_MAX_LEN 128
#endif

#ifndef MQTT_PUBLISH_PAYLOAD_MAX_LEN
#define MQTT_PUBLISH_PAYLOAD_MAX_LEN 1024
#endif

#ifndef MQTT_COMMAND_PAYLOAD_MAX_LEN
#define MQTT_COMMAND_PAYLOAD_MAX_LEN 512
#endif

#ifndef TELEMETRY_INTERVAL_MS
#define TELEMETRY_INTERVAL_MS 1000
#endif

typedef struct
{
    char topic[MQTT_PUBLISH_TOPIC_MAX_LEN];
    char payload[MQTT_PUBLISH_PAYLOAD_MAX_LEN];
    bool retain;
} MQTTMessage_t;

typedef struct
{
    char topic[MQTT_PUBLISH_TOPIC_MAX_LEN];
    char payload[MQTT_COMMAND_PAYLOAD_MAX_LEN];
} MQTTCommand_t;

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);

static TaskHandle_t xTaskMQTTHandle = NULL;
static SemaphoreHandle_t xMutexMQTT = NULL;
static QueueHandle_t xMQTTPublishQueue = NULL;
static QueueHandle_t xMQTTCommandQueue = NULL; // fila para comandos recebidos
static bool forcePublish = false;

static void TaskMQTT_Task(void *pvParameters);
static bool connectWiFi();
static bool connectMQTT();
static void publishLatinhasData_enqueue();
static void mqttCallback(char *topic, byte *payload, unsigned int length);
static const char *estadoToString(EstadoLatas_t estado);

void TaskMQTT_Init()
{
    // Cria mutex
    xMutexMQTT = xSemaphoreCreateMutex();
    if (xMutexMQTT == NULL)
    {
        Serial.println("[MQTT] Erro ao criar mutex");
        return;
    }

    // Cria filas
    xMQTTPublishQueue = xQueueCreate(MQTT_PUBLISH_QUEUE_LENGTH, sizeof(MQTTMessage_t));
    if (xMQTTPublishQueue == NULL)
    {
        Serial.println("[MQTT] Erro ao criar fila de publicação");
        return;
    }

    xMQTTCommandQueue = xQueueCreate(10, sizeof(MQTTCommand_t));
    if (xMQTTCommandQueue == NULL)
    {
        Serial.println("[MQTT] Erro ao criar fila de comandos");
        // não é crítico — podemos continuar sem fila de comandos
    }

    // Configura broker
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(MQTT_KEEPALIVE);

    // Cria task
    BaseType_t result = xTaskCreate(
        TaskMQTT_Task,
        "TaskMQTT",
        TASK_MQTT_STACK_SIZE,
        NULL,
        TASK_MQTT_PRIORITY,
        &xTaskMQTTHandle);

    if (result == pdPASS)
    {
        Serial.println("[MQTT] Task criada com sucesso");
    }
    else
    {
        Serial.println("[MQTT] Erro ao criar task");
    }
}

bool TaskMQTT_IsWiFiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool TaskMQTT_IsMQTTConnected()
{
    return mqttClient.connected();
}

bool TaskMQTT_Send(const char *topic, const char *payload, bool retain)
{
    if (xMQTTPublishQueue == NULL)
    {
        return false;
    }

    MQTTMessage_t msg;
    msg.retain = retain;
    memset(msg.topic, 0, sizeof(msg.topic));
    memset(msg.payload, 0, sizeof(msg.payload));

    strncpy(msg.topic, topic, sizeof(msg.topic) - 1);
    strncpy(msg.payload, payload, sizeof(msg.payload) - 1);

    // Enfileira sem bloquear (tempo zero). Se quiser bloquear, passe um timeout.
    if (xQueueSend(xMQTTPublishQueue, &msg, 0) == pdTRUE)
    {
        return true;
    }

    // fila cheia
    return false;
}

void TaskMQTT_PublishNow()
{
    forcePublish = true;
}

static void TaskMQTT_Task(void *pvParameters)
{
    TickType_t xLastTelemetry = xTaskGetTickCount();
    const TickType_t xTelemetryInterval = pdMS_TO_TICKS(TELEMETRY_INTERVAL_MS);

    Serial.println("[MQTT] Task iniciada");

    // Tenta conectar WiFi inicialmente
    if (!connectWiFi())
    {
        Serial.println("[MQTT] Falha ao conectar WiFi. Task continuará tentando...");
    }

    for (;;)
    {
        // Verifica WiFi e tenta reconectar se necessário
        if (!TaskMQTT_IsWiFiConnected())
        {
            connectWiFi();
            vTaskDelay(pdMS_TO_TICKS(500));
            continue; // espera até a próxima iteração para evitar flood
        }

        // Verifica MQTT e tenta reconectar
        if (!TaskMQTT_IsMQTTConnected())
        {
            connectMQTT();
        }

        // Processa mensagens recebidas do broker
        if (TaskMQTT_IsMQTTConnected())
        {
            if (xSemaphoreTake(xMutexMQTT, pdMS_TO_TICKS(50)) == pdTRUE)
            {
                mqttClient.loop();
                xSemaphoreGive(xMutexMQTT);
            }
        }

        // Publica mensagens que estão na fila de publicação
        MQTTMessage_t pending;
        while (xQueueReceive(xMQTTPublishQueue, &pending, 0) == pdTRUE)
        {
            if (TaskMQTT_IsMQTTConnected())
            {
                if (xSemaphoreTake(xMutexMQTT, pdMS_TO_TICKS(200)) == pdTRUE)
                {
                    bool ok = mqttClient.publish(pending.topic, pending.payload, pending.retain);
                    xSemaphoreGive(xMutexMQTT);

                    if (ok)
                    {
                        Serial.print("[MQTT] Publicado: ");
                        Serial.print(pending.topic);
                        Serial.print(" payload_len=");
                        Serial.println(strlen(pending.payload));
                    }
                    else
                    {
                        Serial.println("[MQTT] Falha ao publicar (re-enfileirando)");
                        // Tenta re-enfileirar sem bloquear
                        xQueueSend(xMQTTPublishQueue, &pending, 0);
                        break; // sai do loop para evitar busy-loop
                    }
                }
                else
                {
                    // Não conseguiu take do mutex: re-enfileira e sai
                    xQueueSend(xMQTTPublishQueue, &pending, 0);
                    break;
                }
            }
            else
            {
                // MQTT desconectado: coloca de volta na fila e tenta reconectar
                xQueueSend(xMQTTPublishQueue, &pending, 0);
                break;
            }
        }

        // Telemetria periódica (publica status público da máquina)
        if (forcePublish || (xTaskGetTickCount() - xLastTelemetry >= xTelemetryInterval))
        {
            publishLatinhasData_enqueue();
            // aqui você pode adicionar outras funções de telemetria, ex:
            // publicarStatusMotor_enqueue();
            xLastTelemetry = xTaskGetTickCount();
            forcePublish = false;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static bool connectWiFi()
{
    Serial.println("[WiFi] Conectando...");
    Serial.print("[WiFi] SSID: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startAttemptTime < WIFI_TIMEOUT_MS)
    {
        Serial.print('.');
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println();
        Serial.println("[WiFi] Conectado!");
        Serial.print("[WiFi] IP: ");
        Serial.println(WiFi.localIP());
        Serial.print("[WiFi] RSSI: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
        return true;
    }
    else
    {
        Serial.println();
        Serial.println("[WiFi] Falha na conexão");
        return false;
    }
}

static bool connectMQTT()
{
    if (!TaskMQTT_IsWiFiConnected())
    {
        return false;
    }

    Serial.println("[MQTT] Conectando ao broker...");
    Serial.print("[MQTT] Broker: ");
    Serial.print(MQTT_BROKER);
    Serial.print(":");
    Serial.println(MQTT_PORT);

    int retries = 0;
    while (!mqttClient.connected() && retries < 5)
    {
        Serial.print("[MQTT] Tentativa ");
        Serial.print(retries + 1);
        Serial.println("/5");

        bool connected = false;
        if (xSemaphoreTake(xMutexMQTT, pdMS_TO_TICKS(200)) == pdTRUE)
        {
            if (strlen(MQTT_USERNAME) > 0)
            {
                connected = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
            }
            else
            {
                connected = mqttClient.connect(MQTT_CLIENT_ID);
            }
            xSemaphoreGive(xMutexMQTT);
        }

        if (connected)
        {
            Serial.println("[MQTT] Conectado ao broker!");

            // Publica status de online
            StaticJsonDocument<128> doc;
            doc["status"] = "online";
            doc["client"] = MQTT_CLIENT_ID;
            doc["timestamp"] = millis();

            char buffer[128];
            serializeJson(doc, buffer);
            TaskMQTT_Send(MQTT_TOPIC_STATUS, buffer, true);

            // Subscrever tópicos necessários
            if (xSemaphoreTake(xMutexMQTT, pdMS_TO_TICKS(200)) == pdTRUE)
            {
                // exemplo: topico de controle remoto
                mqttClient.subscribe("maquina/controle/#");
                xSemaphoreGive(xMutexMQTT);
            }

            return true;
        }
        else
        {
            Serial.print("[MQTT] Falha na conexão. Estado: ");
            Serial.println(mqttClient.state());
            retries++;
            vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY_MS));
        }
    }

    return false;
}

static void publishLatinhasData_enqueue()
{
    uint16_t total = 0;
    const Lata_t *latinhas = Tracking_ObterLista(&total);

    if (total == 0 || latinhas == NULL)
    {
        // ainda assim podemos publicar status vazio se desejado
        return;
    }

    // Monta JSON (cuidado com memória — ajustar tamanhos se necessário)
    const size_t capacity = JSON_ARRAY_SIZE(total) + total * 128 + 256;
    DynamicJsonDocument doc(capacity);
    JsonArray latas = doc.createNestedArray("latas");

    for (uint16_t i = 0; i < total; i++)
    {
        JsonObject lata = latas.createNestedObject();
        lata["id"] = latinhas[i].id;
        lata["etapa"] = estadoToString(latinhas[i].etapa);
        lata["tempoEntrada"] = latinhas[i].tempoEntrada;
        lata["tempoSaida"] = latinhas[i].tempoSaida;
        lata["completa"] = latinhas[i].completa;

        if (!latinhas[i].completa)
        {
            TickType_t tempoAtual = xTaskGetTickCount();
            uint32_t tempoNaEtapa = (tempoAtual - latinhas[i].tempoEntrada) / configTICK_RATE_HZ;
            lata["tempoNaEtapa"] = tempoNaEtapa;
        }
    }

    doc["total"] = total;
    doc["timestamp"] = millis();

    char buffer[MQTT_PUBLISH_PAYLOAD_MAX_LEN];
    size_t len = serializeJson(doc, buffer, sizeof(buffer));

    if (len > 0)
    {
        TaskMQTT_Send(MQTT_TOPIC_LATAS, buffer, false);
    }
}

static void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("[MQTT] Mensagem recebida no tópico: ");
    Serial.println(topic);

    // Coloca os dados na fila de comandos para serem processados pela aplicação
    if (xMQTTCommandQueue != NULL)
    {
        MQTTCommand_t cmd;
        memset(&cmd, 0, sizeof(cmd));
        strncpy(cmd.topic, topic, sizeof(cmd.topic) - 1);

        // Copia o payload com segurança
        unsigned int copyLen = (length < (sizeof(cmd.payload) - 1)) ? length : (sizeof(cmd.payload) - 1);
        memcpy(cmd.payload, payload, copyLen);
        cmd.payload[copyLen] = '\0';

        // Enfileira (se a fila estiver cheia, descarta)
        xQueueSend(xMQTTCommandQueue, &cmd, 0);
    }

    // Opcional: log completo do payload (cuidado com binários)
    // Serial.write(payload, length);
    // Serial.println();
}

static const char *estadoToString(EstadoLatas_t estado)
{
    switch (estado)
    {
    case AGUARDANDOENVASE:
        return "aguardando_envase";
    case ENVASANDO:
        return "envasando";
    case AGUARDANDOTAMPA:
        return "aguardando_tampa";
    case TAMPANDO:
        return "tampando";
    case AGUADANDORECRAVE:
        return "aguardando_recrave";
    case RECRAVANDO:
        return "recravando";
    case FINALIZADA:
        return "finalizada";
    default:
        return "desconhecido";
    }
}

// Fim do arquivo
