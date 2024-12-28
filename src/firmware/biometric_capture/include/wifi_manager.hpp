#ifndef WIFI_MANAGER
#define WIFI_MANAGER

// Class responsible for managing the Wi-Fi connection
class WiFiManager {

  private:
    StateMachine* stateMachine;
    int ledPin;

    // Function to blink the LED
    void blinkLed(int ledPin) {
        bool state = digitalRead(ledPin);
        digitalWrite(ledPin, !state);
    }

    
    int connectionTimeout = 5000;

  public:
      WiFiManager(StateMachine* _stateMachine, int _ledPin) : // Constructor
        ledPin(_ledPin), 
        stateMachine(_stateMachine)
      {
        pinMode(ledPin, OUTPUT);
      }
      
      // Function to connect to the Wi-Fi network
      void connect(const char* ssid, const char* pass) 
      { 
        WiFi.begin(ssid, pass, 6);

        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED) {
          if (millis() - start >= connectionTimeout) {
            Serial.println("Falha ao conectar na rede.");
            return;
          }
          delay(500);
        } 
      }

      // Function to verify the connection status and blink the LED if not connected
      void verifyConnection(int ledPin) {
        try {
          if (WiFi.status() == WL_CONNECTED) {
            digitalWrite(ledPin, HIGH);
            return;
          }

          if (millis() - stateMachine->get_wifiMillis() >= 500) {
            stateMachine->set_wifiMillis(millis());
            blinkLed(ledPin);
          }
        } catch (const std::exception& e) {
          Serial.println("Erro ao verificar conexão WiFi: " + String(e.what()));
        }
      }

      int getWiFiSignalStrength() {
        try {
          return WiFi.RSSI();
        } catch (const std::exception& e) {
          Serial.println("Erro ao obter força do sinal WiFi: " + String(e.what()));
          return 0; // Return 0 as default value in case of error
        }
      }
};

#endif