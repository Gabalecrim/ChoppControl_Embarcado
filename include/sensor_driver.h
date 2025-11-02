#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdbool.h>
#include "driver/gpio.h"
#include "task_leitura.h"

void Sensor_Init(void);

bool Leitura_Sensor(SensorID id);

#endif
