## Monitor de Distância com OLED e MQTT no Raspberry Pi Pico W

Este projeto utiliza um Raspberry Pi Pico W para medir continuamente a distância com um sensor a laser VL53L0X. O valor medido é exibido em tempo real em um display OLED SSD1306 e, simultaneamente, publicado em um broker MQTT através de uma conexão Wi-Fi.

### Funcionalidades
* Leitura de distância precisa utilizando o sensor a laser Time-of-Flight (ToF) VL53L0X.
* Exibição local dos dados em um display OLED de 128x64 pixels.
* Conexão a uma rede Wi-Fi para comunicação com a internet.
* Publicação dos dados de distância em um tópico MQTT para integração com sistemas de automação residencial (como o Home Assistant) ou outras aplicações IoT.
* Código modularizado para fácil manutenção e expansão.

### Hardware Necessário
|          Componente          | Quantidade |                                Descrição                             |
| ---------------------------- | ---------- | -------------------------------------------------------------------- |
|     Raspberry Pi Pico W      |      1     | A placa microcontroladora com Wi-Fi integrado e Display OLED SSD1306 |
| Sensor de Distância VL53L0X  |      1     | Módulo de sensor a laser Time-of-Flight (ToF)                        |

### Estrutura de Pastas
O projeto está organizado da seguinte forma para manter o código limpo e modular:

````
```
|/projeto_final/                                                            
|-- sensor_distancia_laser.c                # Lógica principal e orquestração
|-- sensor_distancia.c                                      
|-- sensor_distancia.h                                      
|-- display_oled.c                                          
|-- display_oled.h                                          
|-- lwipopts.h              # Configurações da pilha de rede
|-- CMakeLists.txt          # Arquivo de build        
|                                                     
|-- /inc/                   # Drivers do display OLED 
|   |-- ssd1306.h     
|   |-- ssd1306_font.h
|   |-- ssd1306_i2c.c
|   |-- ssd1306_i2c.h
|
|-- /src/                   # Módulos de comunicação
|   |-- wifi_conn.c
|   |-- wifi_conn.h
|   |-- mqtt_comm.c
|   |-- mqtt_comm.h
|
|-- /build/                 # Pasta gerada pela compilação
```
````

### Configuração do Ambiente
Para compilar este projeto, você precisa ter o ambiente de desenvolvimento para o Raspberry Pi Pico configurado em sua máquina. Caso não tenha, siga o guia oficial:

Guia Oficial "Getting started with Raspberry Pi Pico"

### Configuração do Projeto
Antes de compilar, você talvez precise ajustar as credenciais de rede e do broker MQTT diretamente no arquivo main.c:

````
```
// sensor_distancia_laser.c

// --- Configurações ---
// Wi-Fi
const char *WIFI_SSID = "Laica-IoT";
const char *WIFI_PASSWORD = "Laica321";

// MQTT Broker
const char *MQTT_BROKER_IP = "200.137.1.176";
const char *MQTT_CLIENT_ID = "pico_danilo_santos";
const char *MQTT_USER = "desafio09";
const char *MQTT_PASSWORD = "desafio09.laica";
const char *MQTT_TOPIC = "ha/desafio09/danilo.santos/dist";
```
````

### Como Compilar e Executar
1. Clone este repositório para a sua máquina.
2. Abra um terminal na pasta raiz do projeto.

3. Crie uma pasta de compilação (build):
````
```
mkdir build
cd build
```
````

4. Execute o CMake para configurar o projeto:
````
```
cmake ..
```
````

5. Inicie a compilação com o Make:
````
```
make
```
````

6. Após a compilação, um arquivo projeto_final.uf2 será gerado dentro da pasta build.

7. Pressione e segure o botão BOOTSEL do seu Pico W e conecte-o ao computador. Ele será montado como um dispositivo de armazenamento.

8. Arraste e solte o arquivo projeto_final.uf2 para dentro do Pico W. A placa irá reiniciar automaticamente e começará a executar o código.

### Verificação
1. No Display OLED: O display deve primeiro mostrar mensagens de status sobre a conexão Wi-Fi e MQTT. Em seguida, ele passará a exibir a distância atual em milímetros, atualizando a cada 5 segundos.
2. No Broker MQTT: Você pode usar um cliente MQTT (como mosquitto_sub, MQTT Explorer, etc.) para se inscrever no tópico e ver os dados chegando.