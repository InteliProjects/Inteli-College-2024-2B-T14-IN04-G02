#include "main.hpp"

const int PUBLISH_FREQUENCY = 3000;
unsigned long timer;
void initializeSerial();
void initializePins();
void handleFingerprintVerification();

void setup() {
    initializeSerial(); // Initialize the serial communication
    initializePins();   // Initialize all pins

    wifiManager = new WiFiManager(&stateMachine, WIFI_LED);

    Serial.println("=====================================");
    Serial.println("Iniciando sistema...");
    Serial.println("=====================================");
}

void initializePins() {
    pinMode(ON_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(HELPER_BUTTON, INPUT);
    pinMode(WRITE_BUTTON, INPUT);

    lcd.init();
    lcd.showConnectingMessage(SSID);
    wifiManager->connect(SSID, PASS);
    lcd.showWelcomeMessage();

    sensorManager = new SensorManager(lcd, BUZZER, &stateMachine);

    mqtt = new MQTT(MQTT_HOST, MQTT_PORT, &client, &stateMachine);

    fpSensor = new FingerprintSensor(&stateMachine, &fpSerial, sensorManager, mqtt, &lcd);
    fpSensor->init();

    storageModule = new StorageModule(mqtt);
    storageModule->begin(SD_PIN);

    storageModule->saveToQueue("a-p0rt4l/callback", "Testando 123");

    storageModule->processQueue();

    digitalWrite(ON_LED, HIGH);

    ubidots.setDebug(true);  // uncomment this to make debug messages available
    ubidots.setCallback(UbidotsManager::callback);
    ubidots.setup();
    ubidots.reconnect();
    timer = millis();
}

void loop() {
    // Verifies the current state of the state machine
    stateMachine.verifyState();

    // Checks if Wi-Fi is connected. Blinks LED if not
    wifiManager->verifyConnection(WIFI_LED);

    // Checks if MQTT is connected. If not, tries to reconnect
    mqtt->verifyConnection();

    // Checks if reset button is pressed
    sensorManager->checkResetMode();

    // Handles fingerprint functions
    handleFingerprintVerification();
}

void initializeSerial() {
    Serial.begin(9600);
    fpSerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(2000);
    Serial.println("=====================================");
    Serial.println("Iniciando sistema...");
    Serial.println("=====================================");
}

void initializePins() {
    pinMode(ON_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(HELPER_BUTTON, INPUT);
    pinMode(RESET_BUTTON, INPUT);
}

void handleFingerprintVerification() {
    int registered_id = fpSensor->verifyFingerprintState();
    if (stateMachine.get_request_id()) { 
        const String data = 
            "{ \"fingerprint_id\": " + String(registered_id) + 
            ", \"request_id\": " + String(stateMachine.get_request_id()) + 
            ", \"action_type\": \"fingerprint_write\"}";
        mqtt->publish(
            CALLBACK_TOPIC, 
            data.c_str());
        stateMachine.set_request_id(0);
    }

    try {
        if (!ubidots.connected()) {
            ubidots.reconnect();
        }
        if (millis() - timer > PUBLISH_FREQUENCY) { // triggers the routine every X seconds
            try {
                int wifi_signal_strength = wifiManager->getWiFiSignalStrength();
                Serial.println(wifi_signal_strength);
                ubidots.add(VARIABLE_LABEL1, wifi_signal_strength); // Inserir o label das suas variáveis
                ubidots.publish(DEVICE_LABEL); //pode usar um único publish para várias variáveis
                timer = millis();
            } catch (const std::exception& e) {
                Serial.println("Erro ao publicar dados no Ubidots: " + String(e.what()));
            }
        }
    } catch (const std::exception& e) {
        Serial.println("Erro ao verificar conexão com Ubidots: " + String(e.what()));
    }
    // ubidots.loop();
}