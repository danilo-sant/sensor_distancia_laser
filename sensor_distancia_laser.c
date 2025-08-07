#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h" // Necessário para o loop do Wi-Fi

// Módulos do nosso projeto
#include "sensor_distancia.h"
#include "display_oled.h"
#include "src/wifi_conn.h"
#include "src/mqtt_comm.h"

// --- Configurações ---
// Wi-Fi
const char *WIFI_SSID = "WIFI_SSID";
const char *WIFI_PASSWORD = "WIFI_PASSWORD";

// MQTT Broker
const char *MQTT_BROKER_IP = "MQTT_BROKER_IP";
const char *MQTT_CLIENT_ID = "MQTT_CLIENT_ID";
const char *MQTT_USER = "MQTT_USER";
const char *MQTT_PASSWORD = "MQTT_PASSWORD";
const char *MQTT_TOPIC = "MQTT_TOPIC";

int main() {
    // Inicializa stdio para depuração
    stdio_init_all();
    sleep_ms(2000);
    printf("Sistema de Monitoramento de Distancia - Iniciando...\n");

    // --- Inicialização dos Módulos ---
    
    // 1. Conectar ao Wi-Fi
    connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);

    // 2. Inicializar o Display OLED
    display_oled_init();
    display_oled_show_text("Conectado!", "Inicializando...");

    // 3. Inicializar o Sensor de Distância
    sensor_distancia_init();

    // 4. Configurar e conectar ao Broker MQTT
    printf("Conectando ao broker MQTT em %s...\n", MQTT_BROKER_IP);
    mqtt_setup(MQTT_CLIENT_ID, MQTT_BROKER_IP, MQTT_USER, MQTT_PASSWORD);
    
    printf("Sistema pronto. Iniciando leituras.\n");
    sleep_ms(2000); // Aguarda a conexão MQTT estabilizar

    // Buffers para formatar as mensagens
    char oled_line_buffer[20];
    char mqtt_payload_buffer[10];

    // --- Loop Principal ---
    while (true) {
        // Bloqueia até que a conexão Wi-Fi esteja ativa
        cyw43_arch_poll();

        int distancia_mm = sensor_distancia_read_mm();

        if (distancia_mm < 0 || distancia_mm > 8000) { // Filtra leituras inválidas
            printf("Erro ou valor invalido na leitura da distancia.\n");
            display_oled_show_text("Erro de", "Leitura");
        } else {
            // Formata os dados para o Display e MQTT
            snprintf(oled_line_buffer, sizeof(oled_line_buffer), "Dist: %d mm", distancia_mm);
            snprintf(mqtt_payload_buffer, sizeof(mqtt_payload_buffer), "%d", distancia_mm);
            
            // Exibe no monitor serial
            printf("Distancia: %s mm | Publicando no tópico: %s\n", mqtt_payload_buffer, MQTT_TOPIC);

            // Exibe no display OLED
            display_oled_show_text("Valor Atual:", oled_line_buffer);

            // Define se esta publicação específica deve ser retida.
            bool reter_esta_mensagem = true; // ou false, dependendo da sua lógica

            // Publica o valor no broker MQTT
            mqtt_comm_publish(MQTT_TOPIC, (const uint8_t *)mqtt_payload_buffer, strlen(mqtt_payload_buffer), reter_esta_mensagem);
        }
        
        // Aguarda 5 segundos antes da próxima leitura e publicação
        sleep_ms(5000);
    }

    return 0;
}