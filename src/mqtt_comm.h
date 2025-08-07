#ifndef MQTT_COMM_H
#define MQTT_COMM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Configura e inicia a conexão com o broker MQTT.
void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass);

// Publica uma mensagem em um tópico MQTT.
void mqtt_comm_publish(const char *topic, const uint8_t *data, size_t len, bool retain);

#endif // MQTT_COMM_H