#include <TinyGPS++.h>
#include "heltec.h"
#define BAND    915E6 //433E6  //you can set band here directly,e.g. 868E6,915E6
TinyGPSPlus gps;
String out;
int x;

void setup()
{
  Serial2.begin(9600, SERIAL_8N1,2,17);
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->clear();
  Heltec.display->drawString(0, 10, "Starting...");
  Heltec.display->display();
  delay(1000);
}

void loop()
{
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      {
      displayInfo();
      LoRa.beginPacket();
      LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
      LoRa.print(out);
      LoRa.endPacket();
      Heltec.display->clear();
      Heltec.display->drawStringMaxWidth(0, 10, 128, out);
      Heltec.display->display();
      }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
    String str = "";
    char buff[12];
    str = str + gps.time.hour() + ":";
    str = str + gps.time.minute() + ":";
    str = str + gps.time.second() + " ";        
    float lat = gps.location.lat();
    dtostrf(lat, 5, 6, buff);
    str = str + buff + " "; 
    float lng = gps.location.lng();
    dtostrf(lng, 5, 6, buff);              
    str = str + buff + " ";
    str = str + gps.altitude.meters() + " ";
    str = str + x;           
    out = str;
    x = x + 1;
}
