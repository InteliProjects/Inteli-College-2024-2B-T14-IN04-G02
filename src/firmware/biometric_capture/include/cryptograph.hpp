#ifndef CRYPTO_MODULE
#define CRYPTO_MODULE

#include <Arduino.h>
#include "mbedtls/aes.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

unsigned char key[32] = {0x05, 0x64, 0x04, 0x65, 0x66, 0x09, 0x65, 0x64, 0x01, 0x06, 
                         0x01, 0x07, 0x02, 0x05, 0x02, 0x02, 0x62, 0x08, 0x02, 0x03, 
                         0x07, 0x05, 0x09, 0x66, 0x09, 0x63, 0x07, 0x00, 0x06, 0x61, 
                         0x05, 0x03};

unsigned char iv[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
                        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

// Class to handle cryptographic operations (AES encryption and decryption)
class Crypto
{
  private:
    mbedtls_aes_context aes; // AES context for encryption and decryption
    int keySyze = 256;       // Key size in bits

  public:
    Crypto() {
      mbedtls_aes_init(&aes);
    }

    // Function to encrypt a message. Returns the size of the encrypted message.
    size_t encryptAES(const unsigned char *input, size_t inputLen, unsigned char *output) {
        unsigned char iv_copy[16];
        memcpy(iv_copy, iv, 16);

        mbedtls_aes_context aes;
        mbedtls_aes_init(&aes);
        mbedtls_aes_setkey_enc(&aes, key, 256);

        size_t outputLen = ((inputLen + 15) / 16) * 16; // Ajusta para múltiplo de 16 bytes
        unsigned char paddedInput[outputLen];
        memcpy(paddedInput, input, inputLen);

        // Aplica padding Pkcs7
        unsigned char padValue = outputLen - inputLen;
        for (size_t i = inputLen; i < outputLen; i++) {
            paddedInput[i] = padValue;
        }

        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, outputLen, iv_copy, paddedInput, output);

        mbedtls_aes_free(&aes);
        return outputLen;
    }

    // Function to decrypt a message. Returns the size of the decrypted message.
    size_t decryptAES(const unsigned char *input, size_t inputLen, unsigned char *output) {
        unsigned char iv_copy[16];
        memcpy(iv_copy, iv, 16);

        mbedtls_aes_context aes;
        mbedtls_aes_init(&aes);
        mbedtls_aes_setkey_dec(&aes, key, 256);

        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, inputLen, iv_copy, input, output);

        // Remove o padding Pkcs7
        unsigned char padValue = output[inputLen - 1];
        size_t actualLen = inputLen - padValue;

        mbedtls_aes_free(&aes);
        return actualLen;
    }

    // Function to print a byte array in hexadecimal format
    void printHex(const unsigned char *data, size_t len) {
        for (size_t i = 0; i < len; i++) {
            Serial.printf("%02x", data[i]);
        }
        Serial.println();
    }

    // Function to convert a hexadecimal string to a byte array
    void hexStringToByteArray(const char *hexString, unsigned char *byteArray, size_t byteArrayLen) {
      for (size_t i = 0; i < byteArrayLen; i++) {
        sscanf(hexString + 2 * i, "%2hhx", &byteArray[i]);
      }
    }

};

#endif