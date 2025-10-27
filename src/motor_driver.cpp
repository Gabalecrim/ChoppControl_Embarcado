#include "motor_driver.h"
#include <Arduino.h>

static const gpio_num_t motor_pin[MOTOR_COUNT] = {
    [MOTOR_ALIMENTACAO] = GPIO_NUM_4,
    [LED_ENVASE] = GPIO_NUM_5,
    [LED_TAMPA] = GPIO_NUM_18,
    [MOTOR_ESTEIRA] = GPIO_NUM_19,
    [ATUADOR_FINAL_ESTEIRA] = GPIO_NUM_20,
    [ATUADOR_GUIA_RECRAVE] = GPIO_NUM_2,
};

void Motor_Init(void)
{
}

bool Aciona_Motor(void)
{
    return true;
}