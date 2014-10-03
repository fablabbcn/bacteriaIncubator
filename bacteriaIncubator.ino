#include "PID_v1.h"
#include "DHT.h"

const int heaterRelayPin = 12;
const int fanRelayPin = 11;
const int tempHumSensorPin = 2;

float hum;
float temp;

void setup() {
  Serial.begin(9600); 
  setMinMaxHeaterOn(3000, 10000);       // set the min and max intervals the heater can be ON in ms.
  updateTargetTemp(37.0);              // set the target temperature in ºC
  updateFanInterval(60000, 10000);     // set the time fan is OFF and and ON in ms. this is for homogenizing the heat. 
  setupSensors();
  setupLogic();
  setupActuators();
}

void loop(){
  updateTempHum();
  computeHeater();
  updateHeater();
  updateFan();
}


