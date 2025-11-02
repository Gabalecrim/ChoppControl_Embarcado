#include "task_leitura.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "sensor_driver.h"
#include "botao_driver.h"

static EstadoSensor_t estado_leitura;
static EstadoSensor_t ultima_leitura;

SemaphoreHandle_t xSemAlimentacao;
SemaphoreHandle_t xSemAlimentacao;

void TaskSensores_Init(void)
{
    // xMutexSensores = xSemaphoreCreateMutex();
    xSemAlimentacao = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(TaskSensores, "Sensores", 2048, NULL, 5, NULL, 1);
}

void TaskSensores(void *pvParameters)
{
    while (true)
    {
        estado_leitura.saida_alimentacao = Leitura_Sensor(SENSOR_SAIDA_ALIMENTACAO);
        // sensores.chegada_envase = Leitura_Sensor(SENSOR_CHEGADA_ENVASE);
        // sensores.passagem_tampa = Leitura_Sensor(SENSOR_PASSAGEM_TAMPA);
        // sensores.final_esteira = Leitura_Sensor(SENSOR_FINAL_ESTEIRA);
        // sensores.chegada_guia_recrave = Leitura_Sensor(SENSOR_CHEGADA_GUIA_RECRAVE);
        estado_leitura.botao_ciclo = Leitura_Botao(BOTAO_START);

        if (estado_leitura.botao_ciclo != ultima_leitura.botao_ciclo)
        {
            xSemaphoreGive(xSemAlimentacao);
        }
        ultima_leitura.botao_ciclo = estado_leitura.botao_ciclo;

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

EstadoSensor_t *Sensores_GetData(void)
{
    return &estado_leitura;
}