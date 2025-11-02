#include "drivers/ponteh_driver.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

static const gpio_num_t motor_pins[MOTOR_COUNT] = {
    [MOTOR_ALIMENTACAO] = GPIO_NUM_2,
    [MOTOR_ESTEIRA] = GPIO_NUM_19,
};

void Motor_Init(void)
{
    for (int i = 0; i < MOTOR_COUNT; i++)
    {
        pinMode(motor_pins[i], OUTPUT);
    }
}

void Aciona_Motor(MotorID id, float potencia)
{
    if (potencia > 0)
    {
        int valor_pwm = 0;

        for (valor_pwm = 0; valor_pwm < potencia; valor_pwm++)
        {
            analogWrite(motor_pins[id], valor_pwm);
            vTaskDelay(pdMS_TO_TICKS(2));
        }
    }
    else
    {
        analogWrite(motor_pins[id], 0);
    }
}