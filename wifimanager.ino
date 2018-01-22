#include <WiFiManager.h>  
/*
#define EEPROM_SALT 12663
typedef struct {
  int   salt = EEPROM_SALT;
} WMSettings;

WMSettings settings;
*/
void wifi_configModeCallback (WiFiManager *myWiFiManager) 
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  
  //entered config mode, make led toggle faster
  led_attach_fast();
}

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void wifi_saveConfigCallback () {
  PRINT("Should save config");
  shouldSaveConfig = true;
}

void wifi_setup()
{ 
  const char *hostname = HOSTNAME;
  
  WiFiManager wifiManager;
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(wifi_configModeCallback);

  //timeout - this will quit WiFiManager if it's not configured in 3 minutes, causing a restart
  wifiManager.setConfigPortalTimeout(180);

/*
  //custom params
  EEPROM.begin(512);
  EEPROM.get(0, settings);
  EEPROM.end();

  if (settings.salt != EEPROM_SALT) {
    Serial.println("Invalid settings in EEPROM, trying with defaults");
    WMSettings defaults;
    settings = defaults;
  }
*/  
  //set config save notify callback
  wifiManager.setSaveConfigCallback(wifi_saveConfigCallback);

  if (!wifiManager.autoConnect(hostname)) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
/*
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("Saving config");

    EEPROM.begin(512);
    EEPROM.put(0, settings);
    EEPROM.end();
  }
  */
}

void wifi_restart() {
  ESP.reset();
  delay(1000);
}

void wifi_reset()
{
  /*
  //reset settings to defaults
  WMSettings defaults;
  settings = defaults;
  EEPROM.begin(512);
  EEPROM.put(0, settings);
  EEPROM.end();
  */
  //reset wifi credentials
  WiFi.disconnect();
  delay(1000);
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  delay(1000);
  ESP.reset();
  delay(1000);
}


