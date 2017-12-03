//TODO: interrupt encoders, look at volatile ints
//TODO: undebugmode segment display output

const int gSpeedR1 = 100; //140,150 for straight
const int gSpeedL1 = 100; //120,140 for straight
const int gSpeedR2 = 0;
const int gSpeedL2 = 0;
const int gSpeedR3 = 0;
const int gSpeedL3 = 0;

#define ENCR 9
#define ENCL 8

#define CYCLEDELAYTIME 1 //ms
#define ROBOTWIDTH 108 //mm , //100 to 116mm
// old wheels: //120.57 +- 25.8mm
#define WHEELRADIUS 32 //mm //32.82mm?

int gLastTimeR = 0;
int gLastTimeL = 0;
double gVR = 0;
double gVL = 0;
double gX = 0;
double gY = 0;
double gAngle = 0;
volatile int gPrevEncStateR = 0;
volatile int gPrevEncStateL = 0;
volatile int gEncTicksR = 0;
volatile int gEncTicksL = 0;
volatile int gEncTicksOldR = 0;
volatile int gEncTicksOldL = 0;
int gStartTime = 0;
int gDisplayState = 0;
int gCurrentSpeedL = 0;
int gCurrentSpeedR = 0;
long gOldTime = 0;

const int gAcceleration = 1000; // set to 1000 to make obsolete

void setup() {
  //Serial.begin(9600);
  
  setupDisplay();
  setupDrive();
  
  pinMode(ENCR,INPUT);
  pinMode(ENCL,INPUT);

  gPrevEncStateR = digitalRead(ENCR);
  gPrevEncStateL = digitalRead(ENCL);
  
  gStartTime = millis();
  gLastTimeR = gStartTime;
  gLastTimeL = gStartTime;
  gOldTime = gStartTime;
}

int getDesiredSpeedL(int thisTime){
  if (thisTime-gStartTime <= 5000) return gSpeedL1;
  else if (thisTime-gStartTime <= 10000) return gSpeedL2;
  else if (thisTime-gStartTime <= 15000) return gSpeedL3;
  return 0;
}

int getDesiredSpeedR(int thisTime){
  if (thisTime-gStartTime <= 5000) return gSpeedR1;
  else if (thisTime-gStartTime <= 10000) return gSpeedR2;
  else if (thisTime-gStartTime <= 15000) return gSpeedR3;
  return 0;
}

void setCurrentSpeed(int desiredSpeedR,int desiredSpeedL){
  //R
  if(gCurrentSpeedR<desiredSpeedR) gCurrentSpeedR = min(gCurrentSpeedR+gAcceleration,desiredSpeedR);
  else if(gCurrentSpeedR>desiredSpeedR) gCurrentSpeedR = max(gCurrentSpeedR-gAcceleration,desiredSpeedR);
  //L
  if(gCurrentSpeedL<desiredSpeedL) gCurrentSpeedL = min(gCurrentSpeedL+gAcceleration,desiredSpeedL);
  else if(gCurrentSpeedL>desiredSpeedL) gCurrentSpeedL = max(gCurrentSpeedL-gAcceleration,desiredSpeedL);
}

void loop() {
  int desiredSpeedR = getDesiredSpeedR(thisTime);
  int desiredSpeedL = getDesiredSpeedL(thisTime);

  // handle acceleration to avoid slipping
  setCurrentSpeed(desiredSpeedR,desiredSpeedL);
  // tell the motors to get busy
  drive(gCurrentSpeedR,gCurrentSpeedL);
  
  //delay
  delay(CYCLEDELAYTIME);

  long thisTime = millis();
  long deltaTime = thisTime-gOldTime; //ms

  // get encoder ticks
  // TODO: change this to an interrupt
  int thisEncStateR = digitalRead(ENCR);
  int thisEncStateL = digitalRead(ENCL);
  if ((gPrevEncStateR != thisEncStateR)){
    if (thisEncStateR == 1){
      gEncTicksR += 1;
    }
    gPrevEncStateR = thisEncStateR;
  }
  if ((gPrevEncStateL != thisEncStateL)){
    if (thisEncStateL == 1){
      gEncTicksL += 1;
    }
    gPrevEncStateL = thisEncStateL;
  }


  gVR = (gEncTicksR-gEncTicksOldR)/(deltaTime*.001);
  gVR = gVR*(PI/10); //rads/s?
  gVR = gVR*WHEELRADIUS; //mm/s

  gVL = (gEncTicksL-gEncTicksOldL)/(deltaTime*.001);
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

  gEncTicksOldR = gEncTicksR;
  gEncTicksOldL = gEncTicksL;

  //chage display state. This code is wonky, but it works.
  if(millis()%1000<=3){
    gDisplayState+=1;
    if(gDisplayState>=3){
      gDisplayState=0;
    }
  }

  double displayValues[3];
  //displayValues[0] = gX; //mm
  //displayValues[1] = gY; //mm
  //displayValues[2] = gAngle*(180/PI); //deg

  displayValues[0] = gEncTicksR;
  displayValues[1] = gEncTicksL;
  displayValues[2] = deltaTime;

  displayOne(displayValues[gDisplayState],gDisplayState);

  gOldTime = thisTime;

  /*
  Serial.print(displayValues[0]);
  Serial.print(" ");
  Serial.println(displayValues[1]);
  Serial.println(displayValues[2]);
  */
}
