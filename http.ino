#include <ESP8266mDNS.h>

/** Current WLAN status */
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void http_setup() {
  server.begin();
}

void http_loop() {
  const char *hostname = HOSTNAME;
  
  int s = WiFi.status();
  if (status != s) { // WLAN status change
    status = s;
    if (s == WL_CONNECTED) {
      // Setup MDNS responder
      if (!MDNS.begin(hostname)) {
        Serial.println("Error setting up MDNS responder!");
      } else {
        Serial.println("mDNS responder started");
        // Add service to MDNS-SD
        MDNS.addService("http", "tcp", 80);
      }
    }
  }

  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    // Wait until the client sends some data
    while(!client.available()){
      delay(1);
    }
   
    // Read the first line of the request
    String request = client.readStringUntil('\r');
    client.flush();
     
    // Match the request
    if (request.indexOf("/on") != -1) {
      turnOn();
    } 
    else if (request.indexOf("/off") != -1){
      turnOff();
    }
    else if (request.indexOf("/toogle") != -1){
      toggle();
    }
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.print(relayState ? "on" : "off");
  }
}

