#ifndef LCD_MODULE
#define LCD_MODULE

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// Class responsible for the comunications and control of the LCD
class LCD {
private:
    LiquidCrystal_I2C lcd;

public:
    LCD(uint8_t addr, uint8_t columns, uint8_t rows) : lcd(addr, columns, rows) {}

    // Inicializa o LCD
    void init() {
        lcd.init();
        lcd.backlight();
    }

    // Limpa o display
    void clear() {
        lcd.clear();
    }

    // Define o cursor do LCD e as linhas a serem exibidas no display
    void print(const String& line1, const String& line2 = "") {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(line1);
        lcd.setCursor(0, 1);
        lcd.print(line2);
    }

    // Exibits the message for connecting to the Wi-Fi
    void showConnectingMessage(const char* ssid) {
        print("Connecting Wifi:", ssid);
        delay(1000);
    }

    // Shows the welcome message
    void showWelcomeMessage() {
        Serial.println("Welcome message");
        print("Coloque seu dedo", "sobre o sensor!");
    }

    // Exibits the message for access granted
    void showAccessGranted() {
        print("Acesso permitido", "Bem-vindo!");
    }

    // Shows the message for requesting help
    void showHelpMessage() {
        print("Solicitando ajuda", "Aguarde...");
    }

    // Exibits the message to wait for pressing help button again
    void showWaitMessage() {
        print("Aguarde 15s para", "solicitar ajuda");
    }

    // Shows the message for reseting the system
    void showResetMessage() {
        print("Resetando sistema,", "Aguarde...");
    }
};
#endif