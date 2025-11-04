#include "drivers/servo_driver.h"
#include <Arduino.h>

static const gpio_num_t servo_pin[SERVO_COUNT] = {
    [ATUADOR_FINAL_ESTEIRA] = GPIO_NUM_32,
    [ATUADOR_GUIA_RECRAVE] = GPIO_NUM_5,
};

static const int SERVO_FREQ = 50; // 50Hz = 20ms
static const int SERVO_RES = 16;  // 16-bit resolution
static const int SERVO_MIN = 1638;
static const int SERVO_MAX = 8192;

void Servo_Init(void)
{
    for (int i = 0; i < SERVO_COUNT; i++)
    {
        ledcSetup(i, SERVO_FREQ, SERVO_RES);
        ledcAttachPin(servo_pin[i], i);
    }
}

void Aciona_Servo(ServoID id, float angulo)
{
    int duty = map(angulo, 0, 180, SERVO_MIN, SERVO_MAX);
    ledcWrite(id, duty);
}