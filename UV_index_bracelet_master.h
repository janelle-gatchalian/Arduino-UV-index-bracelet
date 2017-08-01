/***************************************************
  This UV index bracelet was inspired by the Sunscreen reminder hat using the Si1145 UV/IR/Visible Light Sensor
  visit https://learn.adafruit.com/sunscreen-reminder-hat for full tutorial
  Written by Becky Stern for Adafruit Industries.
  BSD license, all text above must be included in any redistribution.

  EasyNeoPixels.h - Library for making neopixels more approachable.
  Created by Evelyn Masso, April 9, 2017.

  UV index bracelet
  Written by Janelle Gatchalian, June 2017.
 ****************************************************/

#include <Wire.h>
#include "Adafruit_SI1145.h"
#include <EasyNeoPixels.h>

Adafruit_SI1145 uv = Adafruit_SI1145();

uint32_t sinceTime;
uint32_t markerTime; //start time, usually 0
boolean shouldLight = false;
float UVindex;
float lowUVthreshold = .01;
float mediumUVthreshold = 7.1;
float highUVthreshold = 10.1;
//.01-7.0 Green light. Moderate to high risk of UV rays.
//7.1-10.0 Yellow light. Very high risk of UV rays.
//10.1-12 Red light. Extremely high risk of UV rays.
uint32_t reapplyInterval = 900000; // 15 minutes = 900000 milliseconds. One hour = 3600000 milliseconds. Two hours = 7200000 milliseconds

void setup() {
  setupEasyNeoPixels(12, 1);
  Serial.begin(9600);
  Serial.println("Adafruit SI1145 test");
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }
  Serial.println("OK!");
}

void loop() {
  Serial.println("===================");
  UVindex = uv.readUV();
  UVindex /= 100.0; // the index is multiplied by 100 so to get the integer index, divide by 100!
  Serial.print("UV: ");  Serial.println(UVindex);
  Serial.print("Seconds until next alert: ");  Serial.println((reapplyInterval - sinceTime) / 1000);
  delay(1000);

  sinceTime = millis() - markerTime; //how long since last "mark"

  if (UVindex >= highUVthreshold && shouldLight) { //only light when we are currently outside
    for (int i = 0; i <= 3; i++) {
      digitalWrite(12, HIGH);
      writeEasyNeoPixel(0, 180, 0, 0); //(neopixel number, R value, G value, B value)
      delay(250);
      digitalWrite(12, LOW);
      writeEasyNeoPixel(0, 180, 0, 0);
      delay(250);
    }
    Serial.println("===================");
    Serial.println("LIGHT");
    shouldLight = false;
    resetTimer();
  }
  else if (UVindex >= mediumUVthreshold && shouldLight) { //only light when we are currently outside
    for (int i = 0; i <= 3; i++) {
      digitalWrite(12, HIGH);
      writeEasyNeoPixel(0, 255, 200, 0); //(neopixel number, R value, G value, B value)
      delay(250);
      digitalWrite(12, LOW);
      writeEasyNeoPixel(0, 255, 200, 0);
      delay(250);
    }
    Serial.println("===================");
    Serial.println("LIGHT");
    shouldLight = false;
    resetTimer();
  }
  else if (UVindex >= lowUVthreshold && shouldLight) { //only light when we are currently outside
      digitalWrite(12, HIGH);
      writeEasyNeoPixel(0, 0, 180, 0); //(neopixel number, R value, G value, B value)
      delay(250);
      digitalWrite(12, LOW);
      writeEasyNeoPixel(0, 0, 180, 0);
      delay(250);
      digitalWrite(12, HIGH);
      writeEasyNeoPixel(0, 0, 180, 0);
      delay(250);
      digitalWrite(12, LOW);
      writeEasyNeoPixel(0, 0, 180, 0);
      delay(250);
      digitalWrite(12, HIGH);
      writeEasyNeoPixel(0, 0, 180, 0);
      delay(250);
      digitalWrite(12, LOW);
      writeEasyNeoPixel(0, 0, 180, 0);
      delay(250);
    }
    Serial.println("===================");
    Serial.println("LIGHT");
    shouldLight = false;
    resetTimer();
  }
  if (sinceTime > reapplyInterval) { //check to see if we've exceeded the time limit
    shouldLight = true;
    resetTimer();
  }

}

void resetTimer(){
  markerTime = millis();
  sinceTime = 0;
}
