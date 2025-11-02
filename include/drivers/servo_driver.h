#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

typedef enum
{
    ATUADOR_FINAL_ESTEIRA,
    ATUADOR_GUIA_RECRAVE,
    SERVO_COUNT
} ServoID;

void Servo_Init(void);

void Aciona_Servo(ServoID id, float angulo);

#endif