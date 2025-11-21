#include <Arduino.h>
#include "tasks/task_alimentacao.h"
#include "tasks/task_leitura.h"
#include "tasks/task_envase.h"
#include "tasks/task_tampa.h"
#include "tasks/task_recrave.h"
#include "tasks/task_mqtt.h"
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
  Serial.begin(115200);
  delay(1000);
  Serial.println("[SISTEMA] Iniciando ChoppControl...");

  Led_Init();
  Motor_Init();
  Servo_Init();
  TaskSensores_Init();
  TaskAlimentacao_Init();
  TaskEnvase_Init();
  Task_Tampa_Init();
  TaskRecrave_Init();
  TaskMQTT_Init();

  Serial.println("[SISTEMA] Sistema iniciado!");
}

void loop()
{
}
