#include <Wire.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

#define I2CAddressESPWifi 8
int x=32;
String inString;

void setup()
{
  Serial.begin(115200);
  Wire.begin(0,2);//Change to Wire.begin() for non ESP.
  WiFiMulti.addAP("MatiasK Wifi", "argentino123");
}

void send_data(String sensor_value) 
{  
  if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        http.begin("http://api.thingspeak.com/update?api_key=TVXFQUNHZWEBI41U&field1="+sensor_value); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    delay(15000);
}

void loop()
{
  Wire.beginTransmission(I2CAddressESPWifi);
  Wire.write(x);
  Wire.endTransmission();
  x++;
  delay(1);//Wait for Slave to calculate response.
  Wire.requestFrom(I2CAddressESPWifi,10);
  inString = "";
  Serial.print("Request Return:[");
  while (Wire.available())
  {
    delay(1);
    int c = Wire.read();
    if (isDigit(c)) {
      // convert the incoming byte to a char
      // and add it to the string:
      inString += (char)c;
    }
  }
  Serial.print(inString);
  Serial.println("]");
  send_data(inString);
}
