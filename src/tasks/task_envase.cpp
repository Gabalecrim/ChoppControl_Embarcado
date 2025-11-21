#include "tasks/task_envase.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include "drivers/ponteh_driver.h"
#include "drivers/led_driver.h"

static EstadoEnvase_t estado_envase;

void TaskEnvase_Init(void)
{
    xTaskCreatePinnedToCore(task_envase, "Envase", 2048, NULL, 5, NULL, 0);
    estado_envase = AGUARDANDO_LATA;
}

void task_envase(void *pvParameters)
{
    EstadoSensor_t *sensores = Sensores_GetData();
    while (1)
    {
        if (xSemaphoreTake(xSemEnvase, portMAX_DELAY) == pdTRUE)
        {
            if (sensores->botao_ciclo)
            {
                switch (estado_envase)
                {
                case AGUARDANDO_LATA:
                    if (sensores->chegada_envase)
                    {
                        Aciona_Motor(MOTOR_ESTEIRA, 0);
                        Aciona_Led(LED_ENVASE, true);
                        tempo_inicio_envase = xTaskGetTickCount();
                        estado_envase = ENVASE;
                    }
                    else
                        Aciona_Motor(MOTOR_ESTEIRA, 100);
                    break;
                case ENVASANDO:
                    if (xTaskGetTickCount() - tempo_inicio_envase < 5000 / portTICK_PERIOD_MS)
                        break;
                    tempo_inicio_envase = 0;
                    Aciona_Motor(MOTOR_ESTEIRA, 100);
                    Aciona_Led(LED_ENVASE, false);
                    estado_envase = AGUARDANDO_LATA;
                    vTaskDelay(3000 / portTICK_PERIOD_MS);
                    break;
                };
            }
            else
                Aciona_Motor(MOTOR_ESTEIRA, 0);
        }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}