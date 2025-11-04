#include "tasks/task_recrave.h"
#include "tasks/task_leitura.h"
#include "drivers/servo_driver.h"
#include "drivers/led_driver.h"

void TaskRecrave_Init(void)
{
    xTaskCreatePinnedToCore(TaskRecrave, "Recrave", 2048, NULL, 5, NULL, 0);
}

void TaskRecrave(void *pvParameters)
{
    EstadoSensor_t *sensores = Sensores_GetData();
    while (1)
    {
        if (xSemaphoreTake(xSemRecrave, portMAX_DELAY) == pdTRUE)
        {
            if (sensores->botao_ciclo)
            {
                Aciona_Servo(ATUADOR_FINAL_ESTEIRA, 180);
                vTaskDelay(50 / portTICK_PERIOD_MS);
                Aciona_Servo(ATUADOR_GUIA_RECRAVE, 180);
                vTaskDelay(50 / portTICK_PERIOD_MS);
                Aciona_Led(LED_RECRAVE, true);
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                Aciona_Led(LED_RECRAVE, false);
                Aciona_Servo(ATUADOR_GUIA_RECRAVE, 0);
                vTaskDelay(50 / portTICK_PERIOD_MS);
                Aciona_Servo(ATUADOR_FINAL_ESTEIRA, 0);
            }
            else
            {
                Aciona_Servo(ATUADOR_FINAL_ESTEIRA, 0);
                Aciona_Servo(ATUADOR_GUIA_RECRAVE, 0);
                Aciona_Led(LED_RECRAVE, false);
            }
        }

        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}