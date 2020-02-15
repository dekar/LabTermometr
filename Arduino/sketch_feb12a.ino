#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
//#include "esp_log.h"

#include <DallasTemperature.h>

#include <OneWire.h>

#include <ESP8266WebServer.h>


// Data wire is plugged into port D2 on the ESP8266


/*Put your SSID & Password*/
const char* ssid = "ESSID";  // Enter SSID here
//const char* ssid = "";  // Enter SSID here
const char* password = "PWD";  //Enter Password here
IPAddress gateway;

void testSend(uint8_t *dt, uint8_t size)
{
    WiFiClient client;
    //client.connect(gateway, 4848);
    //byte ip[] = { 192, 168, 4, 1};
    
    if(client.connect(gateway, 4848))
    {
      Serial.println("connected");
      client.write("RT");
      while(size--)
        client.write(*dt++);
      client.flush();
      client.stop();
    } else 
    {
      Serial.println("connection failed");
      ESP.restart();
    }
}

void setup() {
  Serial.begin(57600);
  delay(100);
  
  Serial.println("Connecting to ");
  Serial.println(ssid);

  
  //connect to your local wi-fi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  gateway = WiFi.gatewayIP();
  Serial.print("GATEWAY: ");
  Serial.println(gateway);  
  senSetup();
}
uint8_t buf[128];
void loop() {
  uint8_t size = senLoop(buf);
  Serial.print("Send ");
  Serial.print(size);
  Serial.println(" bytes");
  testSend(buf,size);
  delay(15000);
}
