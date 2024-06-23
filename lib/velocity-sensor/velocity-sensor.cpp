#include "velocity-sensor.hpp"
#include <Arduino.h>

#define CHIP_ANALOG 36
#define CHIP_DIGITAL 18

#define ALERT_LED 4

void setupSensor(){
  pinMode(CHIP_ANALOG, INPUT);
  pinMode(CHIP_DIGITAL, INPUT);
}


float measureVelocity(){
  int CHIP_ANALOG_X = analogRead(CHIP_ANALOG);
  int CHIP_ScaledValue = map(CHIP_ANALOG_X,0,4095,0,100);
  return CHIP_ScaledValue;
}