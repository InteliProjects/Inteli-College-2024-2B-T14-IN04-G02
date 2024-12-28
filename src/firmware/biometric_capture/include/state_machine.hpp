#ifndef STATE_MACHINE
#define STATE_MACHINE

#include <Arduino.h>

// StateMachine class to manage global values and control the LED behavior
class StateMachine
{
  private:
    bool write_mode = false;       // Indicates whether the device is in write mode
    long lastPress = 0;           // Stores the timestamp of the last button press
    long helpModeMillis = 0;      // Tracks the timing for help mode (in milliseconds)
    long resetModeMillis = 0;     // Tracks the timing for reset mode (in milliseconds)
    long wifiMillis = 0;          // Tracks the timing for Wi-Fi reconnect attempts (in milliseconds)
    int WRITE_MODE_LED = 33;      // Pin number for the write mode LED
    long request_id = 0;          // Tracks a request ID to identify if a request is active

  public:
    // Functions to get the value of write_mode and set the value of write_mode, respectively.
    bool get_write_mode() { return write_mode; }
    void set_write_mode(bool mode) { write_mode = mode; }

    // Functions to get and set the helpModeMillis timing, respectively. (Will be used when the help button is pressed)
    long get_helpModeMillis() { return helpModeMillis; }
    void set_helpModeMillis(long millis) { helpModeMillis = millis; }

    // Functions to get and set the resetModeMillis timing, respectively. (Will be used when the reset button is pressed)
    long get_resetModeMillis() { return resetModeMillis; }
    void set_resetModeMillis(long millis) { resetModeMillis = millis; }
    
    // Functions to get the value of wifiMillis and set the value of wifiMillis, respectively.
    long get_wifiMillis() { return wifiMillis; }
    void set_wifiMillis(long millis) { wifiMillis = millis; }

    // Getters and setters for request ID
    long get_request_id() { return request_id; }
    void set_request_id(long _request_id) { request_id = _request_id; }

    // Verifies the current state of the system and controls the LED behavior
    void verifyState() {
      if (write_mode && !request_id) {
        // Blink the LED if in write mode and no active request
        blinkWithoutDelay(WRITE_MODE_LED, 500);
      } else {
        // Turn the LED on (steady) if not blinking
        digitalWrite(WRITE_MODE_LED, HIGH);
      }
    }

    // Non-blocking LED blink function using the millis() timer
    void blinkWithoutDelay(int led, long interval) {
      unsigned long currentMillis = millis(); // Get the current time
      static unsigned long previousMillis = 0; // Static variable to store the last blink time
      static bool ledState = LOW;              // Static variable to track the LED state (on/off), that is, LOW or HIGH

      // Check if the interval has passed since the last blink
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Update the last blink time 
        ledState = !ledState;           // Toggle the LED state (on/off), that is, LOW or HIGH
        digitalWrite(led, ledState);    // Update the LED with the new state
      }
    }
};

#endif