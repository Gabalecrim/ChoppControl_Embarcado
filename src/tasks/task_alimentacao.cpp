#include "task_alimentacao.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include "servo_driver.h"

void TaskAlimentacao_Init(void)
{
    Serial.begin(115200);
    xTaskCreatePinnedToCore(task_alimentacao, "Alimentacao", 2048, NULL, 5, NULL, 0);
}

void task_alimentacao(void *pvParameters)
{
    EstadoSensor_t *sensores = Sensores_GetData();
    while (1)
    {
        if ((xSemaphoreTake(xSemAlimentacao, portMAX_DELAY) == pdTRUE))
        {
            if (sensores->botao_ciclo)
            {
                Serial.println("Alimentacao ligada");
                Aciona_Servo(ATUADOR_GUIA_RECRAVE, 180);
            }
            else
            {
                Serial.println("Alimentacao desligada");
                Aciona_Servo(ATUADOR_GUIA_RECRAVE, 0);
            }
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}