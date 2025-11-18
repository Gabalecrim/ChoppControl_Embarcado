#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "tasks/task_mqtt.h"
#include "config.h"
#include "tracking_latas.h"
#include "estado_comum.h"

// ========== Variáveis Globais ==========
static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);
static TaskHandle_t xTaskMQTTHandle = NULL;
static SemaphoreHandle_t xMutexMQTT = NULL;
static bool forcePublish = false;

// ========== Protótipos de Funções Privadas ==========
static void TaskMQTT_Task(void* pvParameters);
static bool connectWiFi();
static bool connectMQTT();
static void publishLatinhasData();
static void mqttCallback(char* topic, byte* payload, unsigned int length);
static const char* estadoToString(EstadoLatas_t estado);

// ========== Implementação das Funções Públicas ==========

void TaskMQTT_Init()
{
    // Cria mutex para proteção de recursos compartilhados
    xMutexMQTT = xSemaphoreCreateMutex();
    if (xMutexMQTT == NULL)
    {
        Serial.println("[MQTT] Erro ao criar mutex");
        return;
    }

    // Configura o broker MQTT
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(MQTT_KEEPALIVE);

    // Cria a task MQTT
    BaseType_t result = xTaskCreate(
        TaskMQTT_Task,
        "TaskMQTT",
        TASK_MQTT_STACK_SIZE,
        NULL,
        TASK_MQTT_PRIORITY,
        &xTaskMQTTHandle
    );

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

bool TaskMQTT_Publish(const char* topic, const char* payload)
{
    if (!TaskMQTT_IsMQTTConnected())
    {
        return false;
    }

    if (xSemaphoreTake(xMutexMQTT, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        bool result = mqttClient.publish(topic, payload, MQTT_RETAIN);
        xSemaphoreGive(xMutexMQTT);
        return result;
    }
    return false;
}

void TaskMQTT_PublishNow()
{
    forcePublish = true;
}

// ========== Implementação das Funções Privadas ==========

static void TaskMQTT_Task(void* pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPublishInterval = pdMS_TO_TICKS(MQTT_PUBLISH_INTERVAL_MS);

    Serial.println("[MQTT] Task iniciada");

    // Conecta ao WiFi
    if (!connectWiFi())
    {
        Serial.println("[MQTT] Falha ao conectar WiFi. Task suspensa.");
        vTaskSuspend(NULL);
    }

    for (;;)
    {
        // Verifica conexão WiFi
        if (!TaskMQTT_IsWiFiConnected())
        {
            Serial.println("[MQTT] WiFi desconectado. Tentando reconectar...");
            connectWiFi();
        }

        // Verifica conexão MQTT
        if (!TaskMQTT_IsMQTTConnected())
        {
            Serial.println("[MQTT] MQTT desconectado. Tentando reconectar...");
            connectMQTT();
        }

        // Processa mensagens MQTT
        if (TaskMQTT_IsMQTTConnected())
        {
            if (xSemaphoreTake(xMutexMQTT, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                mqttClient.loop();
                xSemaphoreGive(xMutexMQTT);
            }

            // Publica dados periodicamente ou sob demanda
            if (forcePublish || (xTaskGetTickCount() - xLastWakeTime >= xPublishInterval))
            {
                publishLatinhasData();
                xLastWakeTime = xTaskGetTickCount();
                forcePublish = false;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
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
        Serial.print(".");
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
    while (!mqttClient.connected() && retries < 3)
    {
        Serial.print("[MQTT] Tentativa ");
        Serial.print(retries + 1);
        Serial.println("/3");

        bool connected = false;
        
        if (xSemaphoreTake(xMutexMQTT, pdMS_TO_TICKS(100)) == pdTRUE)
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
            
            // Publica mensagem de status
            StaticJsonDocument<128> doc;
            doc["status"] = "online";
            doc["client"] = MQTT_CLIENT_ID;
            doc["timestamp"] = millis();
            
            char buffer[128];
            serializeJson(doc, buffer);
            TaskMQTT_Publish(MQTT_TOPIC_STATUS, buffer);
            
            return true;
        }
        else
        {
            Serial.print("[MQTT] Falha na conexão. Estado: ");
            Serial.println(mqttClient.state());
            retries++;
            if (retries < 3)
            {
                vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_DELAY_MS));
            }
        }
    }

    return false;
}

static void publishLatinhasData()
{
    uint16_t total = 0;
    const Lata_t* latinhas = Tracking_ObterLista(&total);

    if (total == 0)
    {
        return;
    }

    // Cria JSON com dados de todas as latinhas
    DynamicJsonDocument doc(2048);
    JsonArray latas = doc.createNestedArray("latas");

    for (uint16_t i = 0; i < total; i++)
    {
        JsonObject lata = latas.createNestedObject();
        lata["id"] = latinhas[i].id;
        lata["etapa"] = estadoToString(latinhas[i].etapa);
        lata["tempoEntrada"] = latinhas[i].tempoEntrada;
        lata["tempoSaida"] = latinhas[i].tempoSaida;
        lata["completa"] = latinhas[i].completa;
        
        // Calcula tempo na etapa atual (em segundos)
        if (!latinhas[i].completa)
        {
            TickType_t tempoAtual = xTaskGetTickCount();
            uint32_t tempoNaEtapa = (tempoAtual - latinhas[i].tempoEntrada) / configTICK_RATE_HZ;
            lata["tempoNaEtapa"] = tempoNaEtapa;
        }
    }

    doc["total"] = total;
    doc["timestamp"] = millis();

    char buffer[2048];
    size_t len = serializeJson(doc, buffer);

    if (len > 0)
    {
        if (TaskMQTT_Publish(MQTT_TOPIC_LATAS, buffer))
        {
            Serial.print("[MQTT] Publicado: ");
            Serial.print(total);
            Serial.println(" latinhas");
        }
        else
        {
            Serial.println("[MQTT] Falha ao publicar dados");
        }
    }
}

static void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("[MQTT] Mensagem recebida no tópico: ");
    Serial.println(topic);

    // Aqui você pode adicionar lógica para processar mensagens recebidas
    // Por exemplo, comandos para controlar a máquina remotamente
}

static const char* estadoToString(EstadoLatas_t estado)
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