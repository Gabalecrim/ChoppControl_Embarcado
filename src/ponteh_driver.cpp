#include "ponteh_driver.h"
#include <Arduino.h>

static const gpio_num_t motor_pin[MOTOR_COUNT] = {
    [MOTOR_ALIMENTACAO] = GPIO_NUM_4,
    [MOTOR_ESTEIRA] = GPIO_NUM_19,
};

void Motor_Init(void)
{
}

bool Aciona_Motor(void)
{
    return true;
}