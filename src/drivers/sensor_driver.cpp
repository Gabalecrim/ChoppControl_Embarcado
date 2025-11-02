#include "drivers/sensor_driver.h"
#include <Arduino.h>

static const gpio_num_t sensor_pins[SENSOR_COUNT] = {
    [SENSOR_SAIDA_ALIMENTACAO] = GPIO_NUM_4,
    [SENSOR_CHEGADA_ENVASE] = GPIO_NUM_21,
    [SENSOR_PASSAGEM_TAMPA] = GPIO_NUM_6,
    [SENSOR_FINAL_ESTEIRA] = GPIO_NUM_17,
    [SENSOR_CHEGADA_GUIA_RECRAVE] = GPIO_NUM_16,
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

    // if (digitalRead(sensor_pins[id]) == LOW)
    // {
    //     Serial.println("Sensor ativo");
    // }
    // else
    // {
    //     Serial.println("Sensor inativo");
    // }
    return digitalRead(sensor_pins[id]);
}
