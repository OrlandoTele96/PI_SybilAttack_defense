#include "heltec.h"

#define BAND 433E6

void setup()
{
  Heltec.begin(true,true,true,true,BAND);
  Serial.println("LoRa device init succeeded");
}

void loop()
{
  
}
