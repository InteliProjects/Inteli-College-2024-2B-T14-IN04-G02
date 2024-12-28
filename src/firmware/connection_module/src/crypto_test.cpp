#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "mbedtls/rsa.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include <string.h>
#include <base64.h>

mbedtls_rsa_context rsa;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_entropy_context entropy;

const char *crypto_pers = "rsa_encrypt";
const char* publicKey = "-----BEGIN PUBLIC KEY-----\n"
                          "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDOiHCfBofRG46k0UmjPh5sTjlV\n"
                          "sHHHQ/YhQ+5caY4rs2UPhTtF9fLQsikpNskqDT2OUwCI077q2NBCl5Ts/9ukVUqb\n"
                          "H9EVfep0Z8XSI6ox8+UZfyvdUCSgyG0StuQ8gbGx4SQ8UAuAVpKVtHa/zK0fS8qZ\n"
                          "A5li0fo3uSy2EwcOtQIDAQAB\n"
                          "-----END PUBLIC KEY-----\n";

const char* privateKey = "-----BEGIN PRIVATE KEY-----\n"
                              "MIICWwIBAAKBgQDOiHCfBofRG46k0UmjPh5sTjlVsHHHQ/YhQ+5caY4rs2UPhTtF\n"
                              "9fLQsikpNskqDT2OUwCI077q2NBCl5Ts/9ukVUqbH9EVfep0Z8XSI6ox8+UZfyvd\n"
                              "UCSgyG0StuQ8gbGx4SQ8UAuAVpKVtHa/zK0fS8qZA5li0fo3uSy2EwcOtQIDAQAB\n"
                              "AoGAG1WcADQTUCXjSmWHpzMOwDtXI8PrPxn7zpnIx5LR1e9TigsSVfdP+HaH85Q0\n"
                              "ahwgAED0gmdx7m+dGp8ovvg696dPcs3lqKeKjR+RH4CfIZyY99vVsYT3D3QeaynP\n"
                              "gM967zgGoE5lUpq+ml9/cV/zjwoEQ/haBKHcKbaTFzEHeVECQQDrTglUEqDwEls3\n"
                              "wBWfxvgy4u2xMNz8YJcaaJxqWpnRejM31prtZfHl/LxPPEP2NCQW9csMPfoL+tXK\n"
                              "JQ3f168xAkEA4LKYh5qUPx+R4+8+kf3TbahaVvwH/SdRrpyKif8P5jaiCvagiUFJ\n"
                              "712N9Cv9E6pHIx0ZrouohyroGzQzeX2exQJAF9NzBlxMh4KUUfTVO7R99UkdIE/+\n"
                              "UF/HbRkinXJHGPryrLjCPz+PWuZYev1zND7bWKfSw46S/xuLAkj2OrAwQQJANL3X\n"
                              "DaVb/MEVRnJGto/ky82Dfv1jhqlvA9RctG15NulF+drP7ce0agoEovacTbxfPD+9\n"
                              "YnoHkcqhdpk3J+IznQJAR2sD4jTYRYKBLbPHvelIaDQbYZrrLC9KAY0Oh3OVy5PQ\n"
                              "bUvy3JNtXhNOx1oQ915w2QUdYW7FySTiAVGD/Hrazg==\n"
                              "-----END PRIVATE KEY-----\n";

// SSID e senha da rede
#define SSID "Wokwi-GUEST"
#define PASS ""

unsigned long wifiMillis = 0;

#define WIFI_LED 32

void verifyWifiConnection();
void toggleLed(int pin);
void sendRequest(String url, String data, String method);

void generateRsaKey();
char* encryptMessage(const char* message);
char* decryptMessage(const unsigned char* encrypted_message, size_t encrypted_len);

long milliForTest = 0;

// =========================================
void setup() {
  Serial.begin(9600);

  // Inicializar RNG e contexto de RSA
  mbedtls_entropy_init(&entropy);
  mbedtls_ctr_drbg_init(&ctr_drbg);
  mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V15, 0);

  pinMode(WIFI_LED, OUTPUT);

  // Tenta conexão com o Wifi
  WiFi.begin(SSID, PASS, 6);

  while (WiFi.status() != WL_CONNECTED) verifyWifiConnection(); 

  long initialTime = millis();

  generateRsaKey();

  milliForTest = millis();
  sendRequest("https://fingerprint-api-psi.vercel.app/arduino_health", "{\"hello\": \"I'm Live!\"}", "GET");
}

void loop() {
  verifyWifiConnection();

  // testa quando recebe um input do serial
  if (milliForTest != 0 && millis() - milliForTest >= 5000) {
    Serial.println("Iniciando teste de criptografia e descriptografia...");
    milliForTest = 0;

    long initialTime = millis();
    char* encrypted = encryptMessage("Mensagem secreta");
    long finalTime = millis();
    Serial.println("Tempo de criptografia: " + String(finalTime - initialTime) + "ms");
    
    String encryptedBase64;
    encryptedBase64 = base64::encode((const uint8_t*)encrypted, 256);
    Serial.println(encryptedBase64);

    sendRequest(
      "https://fingerprint-api-psi.vercel.app/fingerprint_verify", 
      "{\"fingerprint\": \"" + String(encryptedBase64) + "\"}", 
      "POST"
    );
  }
}
// =========================================

// ================ FUNCTIONS ==============
void generateRsaKey() {
  int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)crypto_pers, strlen(crypto_pers));
  if (ret != 0) {
      Serial.println("Falha ao inicializar gerador de números aleatórios!");
      return;
  }

  // Carregar chave pública e privada
  ret = mbedtls_rsa_gen_key(&rsa, mbedtls_ctr_drbg_random, &ctr_drbg, 2048, 65537);
  if (ret != 0) {
      Serial.println("Falha ao gerar chave RSA!");
      return;
  }

  Serial.println("Chave RSA gerada com sucesso!");
}

char* encryptMessage(const char* message) {
  unsigned char* encrypted = (unsigned char*) malloc(256); // Alocar memória para armazenar o resultado criptografado
  size_t encrypted_len = 0;

  // Criptografar usando a chave pública
  int ret = mbedtls_rsa_pkcs1_encrypt(&rsa, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PUBLIC, strlen(message), (const unsigned char *)message, encrypted);
  if (ret != 0) {
    Serial.println("Falha ao criptografar!");
    free(encrypted); // Liberar a memória alocada em caso de falha
    return NULL;
  }

  Serial.println("Mensagem criptografada com sucesso!");
  return (char*) encrypted; // Retornar a mensagem criptografada
}


char* decryptMessage(const unsigned char* encrypted_message, size_t encrypted_len) {
  unsigned char* decrypted = (unsigned char*) malloc(256); // Buffer para armazenar a mensagem descriptografada
  size_t decrypted_len = 0;

  // Descriptografar usando a chave privada
  int ret = mbedtls_rsa_pkcs1_decrypt(&rsa, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PRIVATE, &decrypted_len, encrypted_message, decrypted, 256);
  if (ret != 0) {
    Serial.println("Falha ao descriptografar!");
    free(decrypted); // Liberar a memória alocada em caso de falha
    return NULL;
  }

  decrypted[decrypted_len] = '\0'; // Adicionar o terminador nulo
  Serial.println("Mensagem descriptografada com sucesso!");
  return (char*) decrypted; // Retornar a mensagem descriptografada
}


void verifyWifiConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(WIFI_LED, HIGH);
    return;
  }

  if (millis() - wifiMillis >= 500) {
    wifiMillis = millis();
    toggleLed(WIFI_LED);
  }
}

void toggleLed(int pin) {
  bool state = digitalRead(pin);
  state = !state;
  digitalWrite(pin, state);
}

void sendRequest(String url, String data, String method) {
  if (WiFi.status() != WL_CONNECTED) {  
    return;
  }

  int httpResponseCode = 0;

  HTTPClient http;
  http.begin(url);

  if (method == "POST") {
    http.addHeader("Content-Type", "application/json");
    httpResponseCode = http.POST(data);
  } else {
    httpResponseCode = http.GET();
  }

  if (httpResponseCode > 0) {
    String response = http.getString(); // Obtém a resposta do servidor
    Serial.println(httpResponseCode);   // Código da resposta HTTP
    Serial.println(response);           // Resposta do servidor
  } else {
    Serial.print("Erro na requisição: ");
    Serial.println(httpResponseCode);
  }

  http.end(); // Fecha a conexão
}