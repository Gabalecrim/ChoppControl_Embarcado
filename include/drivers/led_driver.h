#ifndef led_driver_h
#define led_driver_h

typedef enum
{
    LED_ENVASE,
    LED_TAMPA,
    LED_RECRAVE,
    LED_COUNT
} LedID;

void Led_Init(void);

void Aciona_Led(LedID id, bool estado);

#endif