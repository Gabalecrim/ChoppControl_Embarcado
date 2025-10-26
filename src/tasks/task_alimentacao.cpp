#include "task_alimentacao.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void TaskAlimentacao_Init(void)
{
    Serial.begin(115200);
    xTaskCreatePinnedToCore(task_alimentacao, "Alimentacao", 2048, NULL, 5, NULL, 1);
}

void task_alimentacao(void *pvParameters)
{
    EstadoSensor_t *sensores = Sensores_GetData();
    while (1)
    {
        if (sensores->start_processo)
        {
            Serial.println("Alimentacao ligada");
        }
        else
        {
            Serial.println("Alimentacao desligada");
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}