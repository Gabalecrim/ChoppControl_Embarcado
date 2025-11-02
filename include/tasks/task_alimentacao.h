#ifndef TASK_ALIMENTACAO_H
#define TASK_ALIMENTACAO_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "task_leitura.h"

void TaskAlimentacao_Init(void);

void task_alimentacao(void *pvParameters);

#endif