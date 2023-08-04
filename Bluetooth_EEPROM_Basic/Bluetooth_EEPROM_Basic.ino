#include "BluetoothSerial.h"
#include <EEPROM.h>

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

// GPIO where LED is connected to
const int ledPin =  2;

// Handle received messages
int ledState = HIGH;         // the current state of the output pin
String message = "";

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  // Bluetooth device name
  SerialBT.begin("ESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");

  // initialize EEPROM with predefined size
  EEPROM.begin(1);
  // read the last LED state from flash memory
  ledState = EEPROM.read(0);
  // set the LED to the last stored state
  digitalWrite(ledPin, ledState);

}

void loop() {
  // Read received messages (LED control command)
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n') {
      message += String(incomingChar);
    } else {
      message.trim(); // Remove any leading/trailing spaces
      Serial.write(incomingChar);

      if (message == "led_on") {
        digitalWrite(ledPin, HIGH);
        SerialBT.println("LED is ON");
      } else if (message == "led_off") {
        digitalWrite(ledPin, LOW);
        SerialBT.println("LED is OFF");
      } else {
        SerialBT.println("Unknown command: " + message);
      }

      message = ""; // Clear the message variable
    }
  }
}