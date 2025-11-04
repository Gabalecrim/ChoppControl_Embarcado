#include "tasks/task_tampa.h"
#include "tasks/task_leitura.h"
#include "drivers/led_driver.h"

void Task_Tampa_Init(void)
{
    xTaskCreatePinnedToCore(Task_Tampa, "Tampa", 2048, NULL, 5, NULL, 0);
}

void Task_Tampa(void *pvParameters)
{
    EstadoSensor_t *sensores = Sensores_GetData();
    while (1)
    {
        if ((xSemaphoreTake(xSemTampa, portMAX_DELAY) == pdTRUE))
        {
            if (sensores->botao_ciclo)
            {
                Aciona_Led(LED_TAMPA, true);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                Aciona_Led(LED_TAMPA, false);
            }
            else
            {
                Aciona_Led(LED_TAMPA, false);
            }
        }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}