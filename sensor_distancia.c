#include "sensor_distancia.h"
#include <stdio.h>

#define VL53L0X_ADDR 0x29
#define REG_IDENTIFICATION_MODEL_ID   0xC0
#define REG_SYSRANGE_START            0x00
#define REG_RESULT_RANGE_STATUS       0x14
#define REG_RESULT_RANGE_MM           0x1E

static void config_i2c_sensor() {
    i2c_init(I2C_PORT_SENSOR, 100 * 1000);
    gpio_set_function(SDA_PIN_SENSOR, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN_SENSOR, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN_SENSOR);
    gpio_pull_up(SCL_PIN_SENSOR);
}

static int vl53l0x_init_sensor(i2c_inst_t *i2c) {
    uint8_t reg = REG_IDENTIFICATION_MODEL_ID;
    uint8_t id;

    if (i2c_write_blocking(i2c, VL53L0X_ADDR, &reg, 1, true) != 1) return 0;
    if (i2c_read_blocking(i2c, VL53L0X_ADDR, &id, 1, false) != 1) return 0;

    if (id != 0xEE) {
        printf("ID inválido: 0x%02X (esperado: 0xEE)\n", id);
        return 0;
    }
    return 1;
}

void sensor_distancia_init() {
    config_i2c_sensor();
    sleep_ms(100);
    if (!vl53l0x_init_sensor(I2C_PORT_SENSOR)) {
        printf("Falha ao inicializar o VL53L0X.\n");
        while (1);
    }
    printf("Sensor iniciado com sucesso.\n");
}

int sensor_distancia_read_mm() {
    uint8_t cmd[2] = {REG_SYSRANGE_START, 0x01};
    if (i2c_write_blocking(I2C_PORT_SENSOR, VL53L0X_ADDR, cmd, 2, false) != 2) return -1;

    for (int i = 0; i < 100; i++) {
        uint8_t reg = REG_RESULT_RANGE_STATUS;
        uint8_t status;
        if (i2c_write_blocking(I2C_PORT_SENSOR, VL53L0X_ADDR, &reg, 1, true) != 1) return -1;
        if (i2c_read_blocking(I2C_PORT_SENSOR, VL53L0X_ADDR, &status, 1, false) != 1) return -1;
        if (status & 0x01) break;
        sleep_ms(5);
    }

    uint8_t reg = REG_RESULT_RANGE_MM;
    uint8_t buffer[2];
    if (i2c_write_blocking(I2C_PORT_SENSOR, VL53L0X_ADDR, &reg, 1, true) != 1) return -1;
    if (i2c_read_blocking(I2C_PORT_SENSOR, VL53L0X_ADDR, buffer, 2, false) != 2) return -1;

    return (buffer[0] << 8) | buffer[1];
}