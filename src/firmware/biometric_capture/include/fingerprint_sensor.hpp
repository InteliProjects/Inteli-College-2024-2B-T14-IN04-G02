#ifndef FINGERPRINT_SENSOR
#define FINGERPRINT_SENSOR

#include <Adafruit_Fingerprint.h>
#include "state_machine.hpp"
#include <ArduinoJson.h>
#include "fingerprint_exception.hpp"

class FingerprintSensor
{
  private:
    // Private member variables for the StateMachine
    StateMachine* stateMachine;
    HardwareSerial* fpSerial;
    Adafruit_Fingerprint* finger;
    SensorManager* sensorManager;
    MQTT* mqtt;
    LCD* lcd;

    int fingerprintId; // Integer to last fingerprint ID

  public:
    int sensorState = -1;

    // Constructor
    FingerprintSensor(
      StateMachine* stateMachine,
      HardwareSerial* _fpSerial, 
      SensorManager* sensorManager,
      MQTT* mqtt,
      LCD* lcd
    ) :
      stateMachine(stateMachine),
      fpSerial(_fpSerial),
      finger(new Adafruit_Fingerprint(fpSerial)),
      sensorManager(sensorManager),
      mqtt(mqtt),
      lcd(lcd)
    {}

    void init() {
      try {
        // Verifies if the sensor is connected 
        if (!finger->verifyPassword()) {
          throw FingerprintException("Erro: Senha incorreta ou comunicação falhou.");
        }
        Serial.println("Senha verificada, comunicação estabelecida.");
        
        fingerprintId = getLastFingerprintID();
      }
      catch (const FingerprintException& e) {
        Serial.println(e.what());
        // Handle initialization failure (e.g., enter error state)
        lcd->print("Erro no sensor", "de impressão digital");
      }
      catch (...) {
        Serial.println("Erro desconhecido durante a inicialização do sensor de impressão digital.");
        lcd->print("Erro no sensor", "de impressão digital");
      }
    }

    // Function to verify the fingerprint state and return the fingerprint ID
    int verifyFingerprintState() {
      try {
        sensorState = -1;
        if(stateMachine->get_write_mode() && stateMachine->get_request_id()) {
          int registered_id = registerFingerprint(fingerprintId++);
          stateMachine->set_write_mode(false);
          return registered_id;
        }       
          
        return readFingerprint();
      }
      catch (const FingerprintException& e) {
        Serial.println(e.what());
        return -1;
      }
      catch (...) {
        Serial.println("Erro desconhecido ao verificar o estado da impressão digital.");
        return -1;
      }
    }

    // Function to read the fingerprint and return the fingerprint ID
    int readFingerprint() {
      try {
        sensorState = finger->getImage();

        if(sensorState != FINGERPRINT_OK) {
          return -1;
        }

        sensorManager->buzz(300, 0, 0);
        
        if(!convertFpToZP()) {
          throw FingerprintException("Erro ao converter a imagem da impressão digital.");
        }

        return findFingerprint();
      }
      catch (const FingerprintException& e) {
        Serial.println(e.what());
        return -1;
      }
      catch (...) {
        Serial.println("Erro desconhecido ao ler a impressão digital.");
        return -1;
      }
    }

    // Function to register the fingerprint and return the fingerprint ID
    int registerFingerprint(int id) {
      try {
        Serial.print("Pronto para cadastrar a digital com ID #"); Serial.println(id);
        lcd->print("Cadastrando", "impressão digital");
        
        if (!captureFingerprint() || !convertFpToZPWithTryCount(1)) {
          throw FingerprintException("Erro ao capturar a primeira impressão digital para registro.");
        }

        if (!captureFingerprint() || !convertFpToZPWithTryCount(2)) {
          throw FingerprintException("Erro ao capturar a segunda impressão digital para registro.");
        }

        return storeFingerprintModel(id);
        }
      catch (const FingerprintException& e) {
        Serial.println(e.what());
        return -1;
      }
      catch (...) {
        Serial.println("Erro desconhecido ao registrar a impressão digital.");
        return -1;
      }
    }

    // Function to capture the fingerprint. Throws exception if unsuccessful.
    bool captureFingerprint() {
      sensorState = -1;
      while (sensorState != FINGERPRINT_OK) {
        sensorState = finger->getImage();
        if (sensorState == FINGERPRINT_OK) {
          Serial.println("Imagem capturada com sucesso!");
          return true;
        } else if (sensorState == FINGERPRINT_NOFINGER) {
          stateMachine->verifyState();
        } else if (sensorState == FINGERPRINT_PACKETRECIEVEERR) {
          throw FingerprintException("Erro de comunicação durante a captura da impressão digital.");
        } else if (sensorState == FINGERPRINT_IMAGEFAIL) {
          throw FingerprintException("Falha ao capturar a imagem da impressão digital.");
        }
      }
      return false; // Should not reach here
    }

    // Function to find the fingerprint and return the fingerprint ID
    int findFingerprint() {
      sensorState = finger->fingerFastSearch();
      if (sensorState == FINGERPRINT_OK) {
        Serial.print("Impressão digital encontrada! ID #");
        Serial.print(finger->fingerID);
        Serial.print(" com confiança de ");
        Serial.println(finger->confidence);
        lcd->showAccessGranted();
        sensorManager->buzz(300, 0, 0);
        const String data = 
          "{\"id\": " + String(finger->fingerID) + 
          ", \"confidence\": " + String(finger->confidence) + 
          ", \"action_type\": \"fingerprint_read\"" +
          ", \"catraca_id\": \"" + String(ESP.getEfuseMac()) +"\"}";
        mqtt->publish(CALLBACK_TOPIC, data.c_str());
        return finger->fingerID;
      }

      Serial.println("Impressão digital não encontrada.");
      lcd->print("Acesso negado,", "tente novamente!");
      return -1;
    }

    // Function to convert the fingerprint to a template. Throws exception if unsuccessful.
    bool convertFpToZP() {
      sensorState = finger->image2Tz();
      if (sensorState == FINGERPRINT_OK) {
        Serial.println("Imagem convertida em template!");
        return true;
      } else {
        throw FingerprintException("Erro ao converter a imagem em template.");
      }
    }

    bool convertFpToZPWithTryCount(int tryCount) {
      sensorState = finger->image2Tz(tryCount);
      if (sensorState == FINGERPRINT_OK) {
        Serial.println("Imagem convertida em template!");
        return true;
      } else {
        throw FingerprintException("Erro ao converter a imagem em template com tentativas.");
      }
    }

    int storeFingerprintModel(int id) {
      sensorState = finger->createModel();
      if (sensorState == FINGERPRINT_OK) {
        Serial.println("Modelo criado com sucesso!");
      } else {
        throw FingerprintException("Erro ao criar o modelo da impressão digital.");
      }

      // Stores the template in the sensor database
      sensorState = finger->storeModel(id);
      if (sensorState == FINGERPRINT_OK) {
        Serial.println("Impressão digital cadastrada com sucesso!");
        sensorManager->buzz(500, 0, 0);
        return id;
      } else if (sensorState == FINGERPRINT_PACKETRECIEVEERR) {
        throw FingerprintException("Erro de comunicação ao armazenar o modelo.");
      } else if (sensorState == FINGERPRINT_BADLOCATION) {
        throw FingerprintException("ID inválido para armazenamento do modelo.");
      } else if (sensorState == FINGERPRINT_FLASHERR) {
        throw FingerprintException("Erro ao armazenar no banco de dados.");
      }
      throw FingerprintException("Erro desconhecido ao armazenar o modelo da impressão digital.");
    }

    // Function to get the last fingerprint ID
    int getLastFingerprintID() {
    try {
      if(finger->getTemplateCount() != FINGERPRINT_OK) {
        throw FingerprintException("Erro ao obter o número de digitais cadastradas.");
      }

      int totalTemplates = finger->templateCount; // Total of registered fingerprints
      Serial.print("Total de digitais cadastradas: ");
      Serial.println(totalTemplates);

      if (totalTemplates == 0) {
        return -1; // -1 indicates that there are no fingerprints registered
      }

        return totalTemplates; // Last fingerprint ID has the same value as the total of registered fingerprints
      }
      catch (const FingerprintException& e) {
        Serial.println(e.what());
        return -1;
      }
      catch (...) {
        Serial.println("Erro desconhecido ao obter o último ID da impressão digital.");
        return -1;
      }
    }
};

#endif
