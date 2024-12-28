#ifndef MAIN_HEADER
#define MAIN_HEADER

#define COMMAND_TOPIC "a-p0rt4l/command"
#define CALLBACK_TOPIC "a-p0rt4l/callback"

// Endereço do I2C (default = 0x27)
#define I2C_ADDR 0x27
// Pino de dados (Data)
#define SDA_PIN 21
// Pino de tempo (Clock)
#define SCL_PIN 22

// Define as portas de RX e TX do ESP32
#define RX_PIN 16  // Pino RX do ESP32
#define TX_PIN 17  // Pino TX do ESP32

#define SD_PIN 15

// Pinos dos LEDs
#define ON_LED 33
#define WIFI_LED 32

#define BUZZER 14

// Host e Porta do MQTT
#define MQTT_HOST "broker.emqx.io"
// #define MQTT_USERNAME "ESP32_1"
// #define MQTT_PASSWORD "Ap0rt@l123"
#define MQTT_PORT 1883

#include <Wire.h>
#include <sstream>
#include <Adafruit_Fingerprint.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <esp_system.h>
#include <UbidotsEsp32Mqtt.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "lcd.hpp"
#include "sensor_manager.hpp"
#include "state_machine.hpp"
#include "wifi_manager.hpp"
#include "mqtt.hpp"
#include "storage_module.hpp"
#include "fingerprint_sensor.hpp"
#include "cryptograph.hpp"
#include "ubidots_manager.hpp"

// Inicializa a serial para o sensor de impressão digital
HardwareSerial fpSerial(2);  // UART2 do ESP32 (Serial2)

// SSID e senha da rede
#define SSID  "Lucas-A15" //"Inteli.Iot"  // "RedmiTesta" // 
#define PASS  "00000000" //"@Intelix10T#" // "12Teste34" //   

#define UBIDOTS_TOKEN "BBUS-ky881epRB3VIAStogCp1hDWl6Qbs1j"
#define CLIENT_ID "hidra-g02-t14"
#define DEVICE_LABEL "meu_esp32"
#define VARIABLE_LABEL1 "wifi_signal_strength"

LCD lcd(I2C_ADDR, 16, 2);
StorageModule *storageModule;
StateMachine stateMachine = StateMachine();
WiFiManager *wifiManager;
WiFiClient wifiClient;
FingerprintSensor *fpSensor;
SensorManager *sensorManager;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fpSerial);
MQTT *mqtt;
Crypto crypto = Crypto();
PubSubClient client(wifiClient);
Ubidots ubidots(UBIDOTS_TOKEN, CLIENT_ID);

#endif