#include "tasks/task_leitura.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "drivers/sensor_driver.h"
#include "drivers/botao_driver.h"

static EstadoSensor_t estado_leitura;
static EstadoSensor_t ultima_leitura;

SemaphoreHandle_t xSemAlimentacao;
SemaphoreHandle_t xSemEnvase;

TickType_t tempo_inicio_envase;

void TaskSensores_Init(void)
{
    // xMutexSensores = xSemaphoreCreateMutex();
    xSemAlimentacao = xSemaphoreCreateBinary();
    xSemEnvase = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(TaskSensores, "Sensores", 2048, NULL, 5, NULL, 1);
}

void TaskSensores(void *pvParameters)
{
    while (true)
    {
        estado_leitura.saida_alimentacao = Leitura_Sensor(SENSOR_SAIDA_ALIMENTACAO);
        estado_leitura.chegada_envase = Leitura_Sensor(SENSOR_CHEGADA_ENVASE);
        estado_leitura.passagem_tampa = Leitura_Sensor(SENSOR_PASSAGEM_TAMPA);
        estado_leitura.final_esteira = Leitura_Sensor(SENSOR_FINAL_ESTEIRA);
        estado_leitura.chegada_guia_recrave = Leitura_Sensor(SENSOR_CHEGADA_GUIA_RECRAVE);
        estado_leitura.botao_ciclo = Leitura_Botao(BOTAO_START);

        if (estado_leitura.botao_ciclo != ultima_leitura.botao_ciclo)
        {
            xSemaphoreGive(xSemAlimentacao);
            xSemaphoreGive(xSemEnvase);
        }
        ultima_leitura.botao_ciclo = estado_leitura.botao_ciclo;

        if (estado_leitura.chegada_envase && !ultima_leitura.chegada_envase)
        {
            xSemaphoreGive(xSemEnvase);
        }
        ultima_leitura.chegada_envase = estado_leitura.chegada_envase;

        if (tempo_inicio_envase != 0)
        {
            if (xTaskGetTickCount() - tempo_inicio_envase > 5000 / portTICK_PERIOD_MS)
            {
                xSemaphoreGive(xSemEnvase);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

EstadoSensor_t *Sensores_GetData(void)
{
    return &estado_leitura;
}