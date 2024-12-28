#ifndef UBIDOTS_MANAGER_HEADER
#define UBIDOTS_MANAGER_HEADER

class UbidotsManager {
    public:
        static void callback(char* topic, unsigned char* payload, unsigned int length) {
            Serial.print("Received message from topic: ");
            Serial.println(topic);

            Serial.print("Message payload: ");
            for (unsigned int i = 0; i < length; i++) {
                Serial.print((char)payload[i]);
            }
            Serial.println();
        }
};

#endif
// //esse cliente_id é apenas para não sobrepor os tópicos dentro da conta do Ubidots. Ele é importante aqui no código, mas não vai aparecer na sua dashboard do Ubidots.
// Ubidots ubidots(UBIDOTS_TOKEN, CLIENT_ID); //essa linha é novidade
// const int PUBLISH_FREQUENCY = 3000; // Update rate in milliseconds
// unsigned long timer;


// /****************************************
//  * Main Functions
//  ****************************************/
// void setup()
// {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   ubidots.setDebug(true);  // uncomment this to make debug messages available
// //   ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
//   ubidots.setCallback(callback);
//   ubidots.setup();
//   ubidots.reconnect();
//   timer = millis();
// }
// void loop()
// {
//   // put your main code here, to run repeatedly:
//   if (!ubidots.connected())
//   {
//     ubidots.reconnect();
//   }
//   if (millis() - timer > PUBLISH_FREQUENCY) // triggers the routine every X seconds
//   {
//     int value1 = random(10,100);
//     int value2 = random(10,100);
//     Serial.println(value1);
//     Serial.println(value2);
//     ubidots.add(VARIABLE_LABEL1, value1); // Insert your variable Labels and the value to be sent
//     ubidots.add(VARIABLE_LABEL2, value2); // Insert your variable Labels and the value to be sent
//     ubidots.publish(DEVICE_LABEL); //pode usar um único publish para várias variáveis
//     timer = millis();
//   }
//   ubidots.loop();
// }