// #include <Arduino.h>
// #include <WiFi.h>
// #include <HTTPClient.h>
// #include "mbedtls/rsa.h"
// #include "mbedtls/pk.h"

// // SSID e senha da rede
// #define SSID "Wokwi-GUEST"
// #define PASS ""

// unsigned long wifiMillis = 0;

// #define WIFI_LED 32

// void verifyWifiConnection();
// void toggleLed(int pin);
// void sendRequest(String url, String data, String method);

// // Chave pública RSA (substitua pela sua chave pública)
// const char* public_key =  "-----BEGIN PUBLIC KEY-----\n"
//                           "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDOiHCfBofRG46k0UmjPh5sTjlV\n"
//                           "sHHHQ/YhQ+5caY4rs2UPhTtF9fLQsikpNskqDT2OUwCI077q2NBCl5Ts/9ukVUqb\n"
//                           "H9EVfep0Z8XSI6ox8+UZfyvdUCSgyG0StuQ8gbGx4SQ8UAuAVpKVtHa/zK0fS8qZ\n"
//                           "A5li0fo3uSy2EwcOtQIDAQAB\n"
//                           "-----END PUBLIC KEY-----\n";

// // =========================================
// void setup() {
//   Serial.begin(9600);
//   pinMode(WIFI_LED, OUTPUT);

//   // Tenta conexão com o Wifi
//   WiFi.begin(SSID, PASS, 6);

//   while (WiFi.status() != WL_CONNECTED) verifyWifiConnection();  

//   // Inicialize o mbedtls
//   mbedtls_pk_context pk;
//   mbedtls_pk_init(&pk);

//   // Carregar a chave pública
//   if (mbedtls_pk_parse_public_key(&pk, (const unsigned char*)public_key, strlen(public_key) + 1) != 0) {
//     Serial.println("Erro ao carregar a chave pública");
//     return;
//   }

//   // Mensagem para criptografar
//   const char* message = "Mensagem secreta";
//   unsigned char encrypted[256];  // Buffer para a mensagem criptografada
//   size_t olen = 0;

//   // Criptografar a mensagem
//   if (mbedtls_rsa_rsaes_pkcs1_v15_encrypt(mbedtls_pk_rsa(pk), NULL, NULL, MBEDTLS_RSA_PUBLIC, strlen(message), (unsigned char*)message, encrypted) != 0) {
//     Serial.println("Erro ao criptografar");
//     return;
//   }

//   // Converta o buffer criptografado para uma string base64 (opcional)
//   String encrypted_base64 = "";
//   for (size_t i = 0; i < sizeof(encrypted); i++) {
//     encrypted_base64 += String(encrypted[i], HEX);
//   }

//   sendRequest("https://fingerprint-api-psi.vercel.app/fingerprint_verify", "{\"fingerprint\": \"" +encrypted_base64+ "\", \"id\": 123}", "POST");

//   mbedtls_pk_free(&pk);  // Libera a memória usada pelo mbedtls
// }

// void loop() {
//   verifyWifiConnection();
// }
// // =========================================

// // ================ FUNCTIONS ==============

// void generateKeys() {
//   mbedtls_pk_context pk;
//   mbedtls_pk_init(&pk);
//   mbedtls_pk_setup(&pk, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA));

//   // Carregue a chave pública aqui
//   mbedtls_pk_parse_public_key(&pk, (const unsigned char *)public_key, strlen(public_key) + 1);
// }

// void verifyWifiConnection() {
//   if (WiFi.status() == WL_CONNECTED) {
//     digitalWrite(WIFI_LED, HIGH);
//     return;
//   }

//   if (millis() - wifiMillis >= 500) {
//     wifiMillis = millis();
//     toggleLed(WIFI_LED);
//   }
// }

// void toggleLed(int pin) {
//   bool state = digitalRead(pin);
//   state = !state;
//   digitalWrite(pin, state);
// }

// void sendRequest(String url, String data, String method) {
//   if (WiFi.status() != WL_CONNECTED) {  
//     return;
//   }

//   int httpResponseCode = 0;

//   HTTPClient http;
//   http.begin(url);

//   if (method == "POST") {
//     http.addHeader("Content-Type", "application/json");
//     httpResponseCode = http.POST(data);
//   } else {
//     httpResponseCode = http.GET();
//   }

//   if (httpResponseCode > 0) {
//     String response = http.getString(); // Obtém a resposta do servidor
//     Serial.println(httpResponseCode);   // Código da resposta HTTP
//     Serial.println(response);           // Resposta do servidor
//   } else {
//     Serial.print("Erro na requisição: ");
//     Serial.println(httpResponseCode);
//   }

//   http.end(); // Fecha a conexão
// }