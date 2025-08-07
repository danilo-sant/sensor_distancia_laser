#ifndef SENSOR_DISTANCIA_H
#define SENSOR_DISTANCIA_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições de I2C para o sensor
#define I2C_PORT_SENSOR i2c0
#define SDA_PIN_SENSOR 0
#define SCL_PIN_SENSOR 1

void sensor_distancia_init();
int sensor_distancia_read_mm();

#endif // SENSOR_DISTANCIA_H