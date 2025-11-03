#include "drivers/sensor_driver.h"
#include <Arduino.h>

static const gpio_num_t sensor_pins[SENSOR_COUNT] = {
    [SENSOR_SAIDA_ALIMENTACAO] = GPIO_NUM_27,
    [SENSOR_CHEGADA_ENVASE] = GPIO_NUM_13,
    [SENSOR_PASSAGEM_TAMPA] = GPIO_NUM_15,
    [SENSOR_FINAL_ESTEIRA] = GPIO_NUM_22,
    [SENSOR_CHEGADA_GUIA_RECRAVE] = GPIO_NUM_23,
};

void Sensor_Init(void)
{
    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        pinMode(sensor_pins[i], INPUT);
    }
}

bool Leitura_Sensor(SensorID id)
{
    return !digitalRead(sensor_pins[id]);
}
