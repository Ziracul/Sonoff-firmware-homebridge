/*
   https://tzapu.com/sonoff-firmware-boilerplate-tutorial/
   1MB flash sizee

   sonoff header
   1 - vcc 3v3
   2 - rx
   3 - tx
   4 - gnd
   5 - gpio 14

   esp8266 connections
   gpio  0 - button
   gpio 12 - relay
   gpio 13 - green led - active low
   gpio 14 - pin 5 on header

*/
#include <WiFiManager.h>  
//#include <EEPROM.h>
#include <ESP8266HTTPClient.h>

#define SONOFF_BUTTON    0
#define SONOFF_RELAY    12
#define SONOFF_LED      13
#define SONOFF_INPUT    14
//#define PRINT(a) (Serial.println(a))   // Comment this out to disable prints and save space
#define PRINT(a) 
#define HOSTNAME        "sonoff_00"

const int CMD_WAIT = 0;
const int CMD_BUTTON_CHANGE = 1;

int cmd = CMD_WAIT;
int relayState = LOW;
int buttonState = HIGH; //inverted button state

static long startPress = 0;

void setState(int s) {
  digitalWrite(SONOFF_RELAY, s);
  digitalWrite(SONOFF_LED, (s + 1) % 2); // led is active low
}

void turnOff() {
  relayState = LOW;
  setState(relayState);
  PRINT("OFF");
}

void turnOn() {
  relayState = HIGH;
  setState(relayState);
  PRINT("ON");
}

void toggleState() {
  cmd = CMD_BUTTON_CHANGE;
}

void toggle() {
  PRINT("TOGGLE");
  PRINT(relayState);
  relayState = relayState == HIGH ? LOW : HIGH;
  setState(relayState);
}

void setup()
{
  Serial.begin(115200);

  led_setup();
  
  //setup relay
  pinMode(SONOFF_RELAY, OUTPUT);
  turnOff();

  //setup button
  pinMode(SONOFF_BUTTON, INPUT);
  attachInterrupt(SONOFF_BUTTON, toggleState, CHANGE);
  
  wifi_setup();

  //if you get here you have connected to the WiFi
  PRINT("connected...");
  led_detach();
  
  PRINT("setup completed");

  http_setup();
}

void loop()
{
  //web server
  http_loop();
   
  //delay(200);
  //PRINT(digitalRead(SONOFF_BUTTON));
  switch (cmd) {
    case CMD_WAIT:
      break;
    case CMD_BUTTON_CHANGE:
      int currentState = digitalRead(SONOFF_BUTTON);
      if (currentState != buttonState) {
        if (buttonState == LOW && currentState == HIGH) {
          long duration = millis() - startPress;
          if (duration < 1000) {
            PRINT("short press - toggle relay");
            toggle();
          } else if (duration < 5000) {
            PRINT("medium press - reset");
            wifi_restart();
          } else if (duration < 10000) {
            PRINT("long press - reset settings");
            wifi_reset();
          }
        } else if (buttonState == HIGH && currentState == LOW) {
          startPress = millis();
        }
        buttonState = currentState;
      }
      break;
  }
}
