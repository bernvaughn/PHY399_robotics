//TODO: undebugmode segment display output

const long gSpeedR1 = 0; //140,150 for straight
const long gSpeedL1 = 100; //120,140 for straight
const long gSpeedR2 = 0;
const long gSpeedL2 = 0;
const long gSpeedR3 = 0;
const long gSpeedL3 = 0;
// increment (or decrement) speed by this
//  amount until it reaches the desired value
const long gAcceleration = 1000; // set to 1000 to make obsolete

#define ENCR 19 //WAS: 9
#define ENCL 18 //WAS: 8

#define CYCLEDELAYTIME 14 //ms
#define ROBOTWIDTH 108 //mm , //100 to 116mm
// old wheels: //120.57 +- 25.8mm
#define WHEELRADIUS 32 //mm //32.82mm?

// measures ms of last loop iteration
long gLastTimeR = 0;
long gLastTimeL = 0;
// location variables
double gVR = 0;
double gVL = 0;
double gX = 0;
double gY = 0;
double gAngle = 0;
// flags to tell interrupt which direction wheels
//  are spinning
bool gForwardR = 1; // 1 means moved forward last
bool gForwardL = 1; // 0 means moved backward last
// for keeping total ticks. can be negative when
//  backwards movement.
volatile long gTotEncTicksR = 0;
volatile long gTotEncTicksL = 0;
// measures ticks of last loop iteration.
long gEncTicksOldR = 0;
long gEncTicksOldL = 0;
// store millis() from when loop started
long gStartTime = 0;
// index of item to display on 8-segment
long gDisplayState = 0;
// need these because functions can't
//  easily return arrays.
long gCurrentSpeedL = 0;
long gCurrentSpeedR = 0;
// stores millis() of previous loop iteration
long gOldTime = 0;


void setup() {
  //Serial.begin(9600);
  
  setupDisplay();
  setupDrive();
  
  //pinMode(ENCR,INPUT);
  //pinMode(ENCL,INPUT);
  // when ENCR goes from low to high, fire encTickedR. Similar for ENCL.
  attachInterrupt(digitalPinToInterrupt(ENCR), encTickedR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCL), encTickedL, RISING);
  
  gStartTime = millis();
  gLastTimeR = gStartTime;
  gLastTimeL = gStartTime;
  gOldTime = gStartTime;
}

//interrupts
void encTickedR(){
  // interrupt
  // when the encoder goes high
  if (gForwardR == 1) gTotEncTicksR += 1;
  else gTotEncTicksR -= 1;
}
void encTickedL(){
  // interrupt
  // when the encoder goes high
  if (gForwardL == 1) gTotEncTicksL += 1;
  else gTotEncTicksL -= 1;
}


long getDesiredSpeedL(long thisTime){
  if (thisTime-gStartTime <= 5000) return gSpeedL1;
  else if (thisTime-gStartTime <= 10000) return gSpeedL2;
  else if (thisTime-gStartTime <= 15000) return gSpeedL3;
  return 0;
}
long getDesiredSpeedR(long thisTime){
  if (thisTime-gStartTime <= 5000) return gSpeedR1;
  else if (thisTime-gStartTime <= 10000) return gSpeedR2;
  else if (thisTime-gStartTime <= 15000) return gSpeedR3;
  return 0;
}

void setCurrentSpeed(long desiredSpeedR,long desiredSpeedL){
  //R
  if(gCurrentSpeedR<desiredSpeedR) gCurrentSpeedR = min(gCurrentSpeedR+gAcceleration,desiredSpeedR);
  else if(gCurrentSpeedR>desiredSpeedR) gCurrentSpeedR = max(gCurrentSpeedR-gAcceleration,desiredSpeedR);
  //L
  if(gCurrentSpeedL<desiredSpeedL) gCurrentSpeedL = min(gCurrentSpeedL+gAcceleration,desiredSpeedL);
  else if(gCurrentSpeedL>desiredSpeedL) gCurrentSpeedL = max(gCurrentSpeedL-gAcceleration,desiredSpeedL);
}

void loop() {
  long thisTime = millis();
  
  long desiredSpeedR = getDesiredSpeedR(thisTime);
  long desiredSpeedL = getDesiredSpeedL(thisTime);

  // handle acceleration to avoid slipping
  setCurrentSpeed(desiredSpeedR,desiredSpeedL);
  // tell the motors to get busy
  drive(gCurrentSpeedR,gCurrentSpeedL);
  // update forward flags
  if(gCurrentSpeedR>=0) gForwardR = 1;
  else gForwardR = 0;
  if(gCurrentSpeedL>=0) gForwardL = 1;
  else gForwardL = 0;
  
  
  //delay
  delay(CYCLEDELAYTIME);

  long deltaTime = thisTime-gOldTime; //ms

  // get encoder ticks
  long encTicksR = gTotEncTicksR;
  long encTicksL = gTotEncTicksL;

  gVR = (encTicksR-gEncTicksOldR)/(deltaTime*.001);
  gVR = gVR*(PI/10); //rads/s?
  gVR = gVR*WHEELRADIUS; //mm/s

  gVL = (encTicksL-gEncTicksOldL)/(deltaTime*.001);
  gVL = gVL*(PI/10); //rads/s?
  gVL = gVL*WHEELRADIUS; //mm/s
  

  double vel = ((gVR+gVL)/2); //mm/s
  double omega = ((gVR-gVL)/ROBOTWIDTH); //rot/s?

  double k00 = vel*cos(gAngle);
  double k01 = vel*sin(gAngle);
  double k02 = omega;

  double k10 = vel*cos(gAngle+(deltaTime*.001*k02)/2);
  double k11 = vel*sin(gAngle+(deltaTime*.001*k02)/2);
  double k12 = omega;

  double k20 = vel*cos(gAngle+(deltaTime*.001*k12)/2);
  double k21 = vel*sin(gAngle+(deltaTime*.001*k12)/2);
  double k22 = omega;

  double k30 = vel*cos(gAngle+(deltaTime*.001*k22));
  double k31 = vel*sin(gAngle+(deltaTime*.001*k22));
  double k32 = omega;

  gX = gX + ((deltaTime*.001)/6)*(k00+2*(k10+k20)+k30); //mm
  gY = gY + ((deltaTime*.001)/6)*(k01+2*(k11+k21)+k31); //mm
  gAngle = gAngle + ((deltaTime*.001)/6)*(k02+2*(k12+k22)+k32); //radians

  gEncTicksOldR = encTicksR;
  gEncTicksOldL = encTicksL;

  //chage display state. This code is wonky, but it works.
  if(millis()%1000<=CYCLEDELAYTIME+5){
    gDisplayState+=1;
    if(gDisplayState>=3){
      gDisplayState=0;
    }
  }

  double displayValues[3];
  //displayValues[0] = gX; //mm
  //displayValues[1] = gY; //mm
  //displayValues[2] = gAngle*(180/PI); //deg

  displayValues[0] = gTotEncTicksR;
  displayValues[1] = gTotEncTicksL;
  displayValues[2] = deltaTime;

  displayOne(displayValues[gDisplayState],gDisplayState);

  gOldTime = thisTime;

  /*
  Serial.prlong(displayValues[0]);
  Serial.prlong(" ");
  Serial.prlongln(displayValues[1]);
  Serial.prlongln(displayValues[2]);
  */
}
