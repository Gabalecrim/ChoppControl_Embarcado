#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include <Arduino.h>

typedef enum
{
    MOTOR_ALIMENTACAO,
    MOTOR_ESTEIRA,
    MOTOR_COUNT
} MotorID;

void Motor_Init(void);

void Aciona_Motor(MotorID id, float potencia);

#endif