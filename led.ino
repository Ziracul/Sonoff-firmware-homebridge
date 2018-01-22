#include <Ticker.h>

Ticker ticker;  //for LED status

void led_setup() 
{
  //set led pin as output
  pinMode(SONOFF_LED, OUTPUT);
  
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, led_tick);
}

void led_attach_fast()
{
  ticker.attach(0.2, led_tick);
}

void led_detach()
{
  ticker.detach();
  digitalWrite(SONOFF_LED, HIGH);
}

void led_tick()
{
  //toggle state
  int state = digitalRead(SONOFF_LED);  // get the current state of GPIO1 pin
  digitalWrite(SONOFF_LED, !state);     // set pin to the opposite state
}
