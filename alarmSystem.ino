#include "namespaces.h"
using machine::isShaking;
using namespace sound;
using namespace sensor;

namespace powerPin{
  constexpr uint8_t high[] {2,4};
  constexpr uint8_t low[] {3,5};
};
void powerOn(){
  Serial.begin(115200);
  for(const auto& pin: powerPin::high){
    pinMode(pin,OUTPUT);
    digitalWrite(pin, HIGH);
  }
  for(const auto& pin: powerPin::low){
    pinMode(pin,OUTPUT);
    digitalWrite(pin, LOW);
  }
};
void setup() { 
  powerOn();
}

namespace sensor::pin{
  constexpr uint8_t speaker{11};
  constexpr uint8_t vibration{A0};
  constexpr uint8_t tilt{A1};
}
void loop() {
  tilt = analogRead(sensor::pin::tilt); 
  vibration = analogRead(sensor::pin::vibration); 
  sensor::printValues(tilt, vibration);

  machine::shaking = isShaking(tilt, vibration); 
  if(machine::shaking && !sound::playAlarm){
    startTime = millis();
    alarm::play();
  }
  timerUp = millis() > startTime+alarm::duration;
  if(sound::playAlarm && timerUp)
    alarm::stop();
  sensor::update();
}