#include <Arduino.h>
#include "tasks/task_alimentacao.h"
#include "tasks/task_leitura.h"
#include "tasks/task_envase.h"
#include "drivers/servo_driver.h"
#include "drivers/ponteh_driver.h"
#include "drivers/led_driver.h"

char slot;
unsigned long tempo = 0;
unsigned long auxiliar = 0;
int Intervalo;
void ResetaTimer(int tempo);
void AguardaTimer();

void setup()
{
  Led_Init();
  Motor_Init();
  Servo_Init();
  TaskSensores_Init();
  TaskAlimentacao_Init();
  TaskEnvase_Init();
}

void loop()
{
}
