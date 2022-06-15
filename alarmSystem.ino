#include <Arduino.h>
#include <PCM.h>
#include "namespaces.h"
using namespace sensor;
using namespace sound;
using machine::isShaking;
namespace powerPin{
  constexpr uint8_t high[] = {2,4};
  constexpr uint8_t ground[] = {3,5};
}
void sensor::powerOnAll(){
  Serial.begin(115200);
  for(const auto& pin:  powerPin::high){
    pinMode(pin,OUTPUT);
    digitalWrite(pin, HIGH);
  }
  for(const auto& pin:  powerPin::ground){
    pinMode(pin,OUTPUT);
    digitalWrite(pin, LOW);
  }
};

void setup() { 
  sensor::powerOnAll();
}
void loop() {
  tilt = analogRead(sensor::pin::tilt); 
  vibration = analogRead(sensor::pin::vibration); 
  sensor::printValues(tilt, vibration);

  machine::shaking = isShaking(tilt, vibration, prev_vibration); 
  if(machine::shaking && !sound::playAlarm){
    startTime = millis();
    startPlayback(alarm::recording, alarm::size);
    playAlarm = true;
  }
  timerUp = millis() > startTime+alarm::duration;
  if(sound::playAlarm && timerUp){
    stopPlayback();
    playAlarm = false;
  }

  prev_vibration = vibration;
  prev_tilt = tilt;
  delay(2); 
}