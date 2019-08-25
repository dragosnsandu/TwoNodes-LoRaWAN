#include <TheThingsNetwork.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// Set your AppEUI and AppKey
const char *appEui = "70B3D57ED001AD48";
const char *appKey = "DB9358F2E98C182BB6F50D3A88585492";

#define loraSerial Serial1
#define debugSerial Serial

#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
SSD1306AsciiAvrI2c oled;

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  #if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.set2X();
  oled.println("Joining");
  oled.println("TTN");

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  // Set callback for incoming messages
  ttn.onMessage(message);

  //  debugSerial.println("-- STATUS");
  ttn.showStatus();

  //  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
  oled.clear();
  oled.set2X();
  oled.println("Joined");
}

void loop()
{
  debugSerial.println("-- LOOP");

  // Send single byte to poll for incoming messages
  ttn.poll();

  delay(5000);
  
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(size) + " bytes on port " + String(port) + ":");

  for (int i = 0; i < size; i++)
  {
    debugSerial.print(" " + String(payload[i]));
  }

  debugSerial.println();

  oled.clear();
  oled.println("Temperatura");
  oled.setCursor(40, 20);
  oled.print(String(payload[0]));
  oled.println("C");
  oled.println("Umiditate");
  oled.setCursor(40, 25);
  oled.print(String(payload[1]));
  oled.println("%");
}
