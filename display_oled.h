#ifndef DISPLAY_OLED_H
#define DISPLAY_OLED_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições de I2C para o display
#define I2C_PORT_DISPLAY i2c1
#define SDA_PIN_DISPLAY 14
#define SCL_PIN_DISPLAY 15

void display_oled_init();
void display_oled_clear();
void display_oled_show_text(const char* line1, const char* line2);

#endif // DISPLAY_OLED_H