#ifndef SENSOR_MANAGER
#define SENSOR_MANAGER

#define RESET_BUTTON 5
#define HELPER_BUTTON 4

#include "lcd.hpp"
#include "state_machine.hpp"
#include <Arduino.h>

long lastPress = 0;

// Class responsible for managing the sensors and the help button
class SensorManager {
    // Private member variables for the LCD, buzzer, and state machine
    private:
        LCD& lcd;
        int buzzerPin;
        StateMachine* stateMachine;
    
    // Public methods for the SensorManager class
    public:
        // Constructor
        SensorManager(LCD& display, int buzzer, StateMachine* stateMachine) 
        : lcd(display), buzzerPin(buzzer), stateMachine(stateMachine) {}

        // Function to read the value of the Helper button and handle the help mode 
        void checkHelpButton() {
            bool isPress = digitalRead(HELPER_BUTTON);

            long timeToHelp = millis() - stateMachine->get_helpModeMillis();
            long waitToHelp = 15000 - timeToHelp;

            if (timeToHelp <= 15000 && stateMachine->get_helpModeMillis() > 0) {
                stateMachine->set_write_mode(0);
                lcd.showWaitMessage();
                delay(2000);
                return;
            }

            stateMachine->set_write_mode(0);
            stateMachine->set_helpModeMillis(millis());
            lcd.showHelpMessage();
            buzz(300, 100, 300);
            delay(2000);
            
        }

        // Function to read the value of the Reset button and handle the reset mode
        void checkResetMode() {
            bool isPress = digitalRead(RESET_BUTTON);
            if (!isPress) {
                long timeToHelp = millis() - stateMachine->get_resetModeMillis();
                long waitToHelp = 5000 - timeToHelp;

                if (timeToHelp <= 5000 && stateMachine->get_resetModeMillis() > 0) {
                    stateMachine->set_write_mode(0);
                    lcd.showWaitMessage();
                    delay(2000);
                    return;
                }
                
                stateMachine->set_write_mode(false);
                stateMachine->set_resetModeMillis(millis());
                lcd.showResetMessage();
                buzz(300, 100, 300);
                esp_restart();
                delay(2000);          
            }
            
        }

        // Function to buzz the buzzer for a duration of 200ms with a pause of 0ms and a second duration of 0ms
        void buzz(int duration1 = 200, int pause = 0, int duration2 = 0) {
            digitalWrite(buzzerPin, HIGH);
            delay(duration1);
            digitalWrite(buzzerPin, LOW);
            if (pause > 0) delay(pause);
            if (duration2 > 0) {
                digitalWrite(buzzerPin, HIGH);
                delay(duration2);
                digitalWrite(buzzerPin, LOW);
            }
        }

        // Function to return the LCD object, which is a private member variable
        LCD getLCD() {
            return lcd;
        }
};

#endif