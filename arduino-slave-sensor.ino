#include <Wire.h>
#define I2CAddressESPWifi 8

char response[11] ="0123456789";

void setup()
{
  Serial.begin(115200);
  Wire.begin(I2CAddressESPWifi);
  Wire.onReceive(espWifiReceiveEvent);
  Wire.onRequest(espWifiRequestEvent);
}

void loop()
{
  delay(1);
}

void espWifiReceiveEvent(int count)
{
  Serial.print("Received[");
  while (Wire.available())
  {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println("]");
  //calc response.
}

void espWifiRequestEvent()
{
  Wire.write(response);
}
