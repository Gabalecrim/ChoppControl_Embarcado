#include "tasks/task_alimentacao.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include "drivers/ponteh_driver.h"

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
                Aciona_Motor(MOTOR_ALIMENTACAO, 100);
            }
            else
            {
                Serial.println("Alimentacao desligada");
                Aciona_Motor(MOTOR_ALIMENTACAO, 0);
            }
        }
    }

    vTaskDelay(5 / portTICK_PERIOD_MS);
}