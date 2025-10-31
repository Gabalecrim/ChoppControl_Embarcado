#include <Arduino.h>
#include "task_alimentacao.h"
#include "task_sensores.h"
#include "servo_driver.h"

char slot;
unsigned long tempo = 0;
unsigned long auxiliar = 0;
int Intervalo;
void ResetaTimer(int tempo);
void AguardaTimer();

void setup()
{
  Servo_Init();
  TaskAlimentacao_Init();
  TaskSensores_Init();
}

void loop()
{
}
