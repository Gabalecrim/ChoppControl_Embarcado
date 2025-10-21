#include <Arduino.h>

char slot;
unsigned long tempo = 0;
unsigned long auxiliar = 0;
int Intervalo;
void ResetaTimer(int tempo);
void AguardaTimer();

void setup()
{
  Serial.begin(115200);
  slot = 0;
}

void loop()
{
  ResetaTimer(60000000);

  switch (slot)
  {
  case 0:
    Serial.println("Slot 0");
    slot = 1;
    break;
  case 1:
    Serial.println("Slot 1");
    slot = 2;
    break;
  case 2:
    Serial.println("Slot 2");
    slot = 3;
    break;
  case 3:
    Serial.println("Slot 3");
    slot = 0;
    break;
  }

  Serial.println("- - - --- - - -");

  AguardaTimer();
}

void ResetaTimer(int tempo)
{
  tempo = millis();
  Intervalo = tempo;
}

void AguardaTimer()
{
  auxiliar = millis();
  while (auxiliar - tempo < Intervalo)
  {
    auxiliar = millis();
  }
}