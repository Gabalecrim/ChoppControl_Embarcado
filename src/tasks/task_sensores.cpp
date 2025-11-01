#include "task_sensores.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "sensor_driver.h"
#include "botao_driver.h"

static EstadoSensor_t sensores;

static void TaskSensores(void *pvParameters)
{

    while (true)
    {
        sensores.saida_alimentacao = Leitura_Sensor(SENSOR_SAIDA_ALIMENTACAO);
        // sensores.chegada_envase = Leitura_Sensor(SENSOR_CHEGADA_ENVASE);
        // sensores.passagem_tampa = Leitura_Sensor(SENSOR_PASSAGEM_TAMPA);
        // sensores.final_esteira = Leitura_Sensor(SENSOR_FINAL_ESTEIRA);
        // sensores.chegada_guia_recrave = Leitura_Sensor(SENSOR_CHEGADA_GUIA_RECRAVE);

        sensores.start_processo = Leitura_Botao(BOTAO_START);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void TaskSensores_Init(void)
{
    xTaskCreatePinnedToCore(TaskSensores, "Sensores", 2048, NULL, 5, NULL, 1);
}

EstadoSensor_t *Sensores_GetData(void)
{
    return &sensores;
}
