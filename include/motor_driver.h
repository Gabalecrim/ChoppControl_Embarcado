#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include <Arduino.h>

typedef enum
{
    MOTOR_ALIMENTACAO,
    LED_ENVASE,
    LED_TAMPA,
    MOTOR_ESTEIRA,
    ATUADOR_FINAL_ESTEIRA,
    ATUADOR_GUIA_RECRAVE,
    MOTOR_COUNT
} MotorID;

void Motor_Init(void);

bool Aciona_Motor(void);

#endif