#include "drivers/led_driver.h"
#include "Arduino.h"

static const gpio_num_t led_pins[LED_COUNT] = {
    [LED_ENVASE] = GPIO_NUM_18,
    [LED_TAMPA] = GPIO_NUM_19,
    [LED_RECRAVE] = GPIO_NUM_21,
};

void Led_Init(void)
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        pinMode(led_pins[i], OUTPUT);
        digitalWrite(led_pins[i], LOW);
    }
}

void Aciona_Led(LedID id, bool estado)
{
    digitalWrite(led_pins[id], estado);
}