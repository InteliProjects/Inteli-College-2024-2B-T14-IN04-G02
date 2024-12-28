#ifndef MQTT_MODULE
#define MQTT_MODULE

#include <ArduinoJson.h>
#include "state_machine.hpp"

// Function to be called when a message is received
void callback(char* topic, byte* message, unsigned int length);

class MQTT
{
  private:
    PubSubClient* client;
    const char* mqtt_server;
    int mqtt_port;
    StateMachine* stateMachine;

  public:
    // Constructor
    MQTT(const char* mqtt_server, int mqtt_port, PubSubClient* _pubSubClient, StateMachine* _stateMachine)  
      : client(_pubSubClient), stateMachine(_stateMachine) {
      this->mqtt_server = mqtt_server;
      this->mqtt_port = mqtt_port;

      client->setServer(mqtt_server, mqtt_port);
      client->setCallback([this](char* topic, byte* message, unsigned int length) {
        this->callback(topic, message, length);  
      });
    }

    // Function to return the PubSubClient object
    PubSubClient* getClient() {
      return client;
    }

    // Function to verify the connection to the MQTT broker. If not connected, it will try to reconnect, otherwise it will keep the connection alive.
    void verifyConnection() {
      if (!client->connected()) {
        reconnect();
      }
      client->loop();
    }

    // Function for publishing a message to a topic
    bool publish(const char* topic, const char* message) {
      if (client->connected()) {
        return client->publish(topic, message);
      }
      return false;
    }

    // Function to reconnect to the MQTT broker
    void reconnect() {
      String clientId = "ESP32_Client";
      Serial.print("Conectando ao broker MQTT...");
      if (client->connect(clientId.c_str())) {
        Serial.println("Conectado.");
        client->subscribe(COMMAND_TOPIC, 1);
      } else {
        Serial.print("Falha na conexão, rc=");
        Serial.print(client->state());
        Serial.println(" Tentando novamente, aguarde");
      }
    }

    // Function to verify the message received and set the "write_mode" to true if "mode" is "register".
    bool verifyCommand(const String& message) {
      // Converte a mensagem em JSON
      JsonDocument jsonDoc;
      DeserializationError error = deserializeJson(jsonDoc, message.c_str());
      if (error) {
        Serial.print("Erro ao converter em JSON: ");
        Serial.println(error.c_str());
        return false;
      }

      const char* mode = jsonDoc["mode"]; 

      if(mode && strcmp(mode,"register") == 0) {
        stateMachine->set_write_mode(true);
        stateMachine->set_request_id(jsonDoc["request_id"]);
        Serial.print("Request ID: "); 
        Serial.println(stateMachine->get_request_id());
        Serial.println("O sensor está em modo cadastro: " + stateMachine->get_write_mode());
        return true;
      }

      return false;
    };

    // Function to be called when a message is received
    void callback(char* topic, byte* message, unsigned int length) {
      Serial.print("Mensagem recebida: ");
      Serial.println(topic);

      String msg;
      for(int i = 0; i < length; i++) {
        msg += (char)message[i];
      }
      Serial.println(msg);
      Serial.println();

      verifyCommand(msg);

    }
};

#endif