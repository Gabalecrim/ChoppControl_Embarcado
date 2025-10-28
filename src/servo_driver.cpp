#include "servo_driver.h"
#include <Arduino.h>

static const gpio_num_t servo_pin[SERVO_COUNT] = {
    [ATUADOR_FINAL_ESTEIRA] = GPIO_NUM_4,
    [ATUADOR_GUIA_RECRAVE] = GPIO_NUM_19,
};

void Servo_Init(void)
{
    for (int i = 0; i < SERVO_COUNT; i++)
    {
        pinMode(servo_pin[i], OUTPUT);
        ledcAttachPin(servo_pin[i], i);
        ledcSetup(i, 1000, 10);
    }
}

void Aciona_Servo(ServoID id, uint16_t duty_cycle)
{
    ledcWrite(id, duty_cycle);
}