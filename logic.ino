/********************************************************
 * PID RelayOutput Example
 * Same as basic example, except that this time, the output
 * is going to a digital pin which (we presume) is controlling
 * a relay.  the pid is designed to Output an analog value,
 * but the relay can only be On/Off.
 *
 *   to connect them together we use "time proportioning
 * control"  it's essentially a really slow version of PWM.
 * first we decide on a window size (5000mS say.) we then 
 * set the pid to adjust its output between 0 and that window
 * size.  lastly, we add some logic that translates the PID
 * output into "Relay On Time" with the remainder of the 
 * window being "Relay Off Time"
 ********************************************************/


//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
//PID tempControl(&Input, &Output, &Setpoint,2,5,1, DIRECT);
//PID tempControl(&Input, &Output, &Setpoint, 1, 0.05, 0.25, DIRECT);
PID tempControl(&Input, &Output, &Setpoint, 4, 0.2, 1, DIRECT);

int WindowSize = 0;
int WindowsMin = 0;

unsigned long windowStartTime;

long previousFanMillis = 0; 
long fanInterval = 0;
long fanOnInterval = 0;

void updateTargetTemp(double targetTemp) {
  Setpoint = targetTemp;
}

void updateFanInterval(long timeOff, long timeOn) {
  fanInterval = timeOff;
  fanOnInterval = timeOn;
}

void setMinMaxHeaterOn(int minOn, int maxOn) {
  WindowsMin = minOn;
  WindowSize = maxOn;
}

void setupLogic() {

  windowStartTime = millis();

  //initialize the variables we're linked to  
  //tell the PID to range between 0 and the full window size
  tempControl.SetOutputLimits(WindowsMin, WindowSize);

  //turn the PID on
  tempControl.SetMode(AUTOMATIC);
}

void computeHeater(){
  Input = temp;
  tempControl.Compute();
}

void setupActuators() {
  pinMode(heaterRelayPin, OUTPUT);     
  pinMode(fanRelayPin, OUTPUT);     

}

void updateHeater(){
  if(millis() - windowStartTime > WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if (Output == WindowsMin) {
    digitalWrite(heaterRelayPin, LOW);
  } 
  else if(Output < millis() - windowStartTime) {
    digitalWrite(heaterRelayPin, HIGH); 
  } 
  else {
    digitalWrite(heaterRelayPin, LOW);
  }
}  

void updateFan(){
  unsigned long currentMillis = millis();
  if(currentMillis - previousFanMillis  > fanInterval) {
    digitalWrite(fanRelayPin, HIGH);
    if(currentMillis - previousFanMillis  > fanInterval + fanOnInterval) {
      previousFanMillis = currentMillis;   
      digitalWrite(fanRelayPin, LOW);
    } 
  } 
}











