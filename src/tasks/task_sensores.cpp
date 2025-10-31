#include "task_sensores.h"
#include "sensor_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static EstadoSensor_t sensores;

static void TaskSensores(void *pvParameters)
{
    bool ultimo_estado_botao = false;
    TickType_t ultimo_tempo_mudanca = 0;
    const TickType_t debounce_delay = pdMS_TO_TICKS(50);

    while (true)
    {
        sensores.saida_alimentacao = Leitura_Sensor(SENSOR_SAIDA_ALIMENTACAO);
        sensores.chegada_envase = Leitura_Sensor(SENSOR_CHEGADA_ENVASE);
        sensores.passagem_tampa = Leitura_Sensor(SENSOR_PASSAGEM_TAMPA);
        sensores.final_esteira = Leitura_Sensor(SENSOR_FINAL_ESTEIRA);
        sensores.chegada_guia_recrave = Leitura_Sensor(SENSOR_CHEGADA_GUIA_RECRAVE);
        bool leitura_atual = Leitura_Sensor(BOTAO_START);

        if (leitura_atual != ultimo_estado_botao)
        {
            ultimo_tempo_mudanca = xTaskGetTickCount();
            ultimo_estado_botao = leitura_atual;
        }

        if (leitura_atual && (xTaskGetTickCount() - ultimo_tempo_mudanca) > debounce_delay)
        {
            sensores.start_processo = !sensores.start_processo;

            while (!Leitura_Sensor(BOTAO_START))
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }

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
