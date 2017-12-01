#define ENCR 9
#define ENCL 8

#define CYCLEDELAYTIME 5 //ms
#define ROBOTWIDTH 133.47 //mm, //120.57 +- 25.8mm
#define WHEELRADIUS 32.82 //mm //32.82mm?

int gLastTimeR = 0;
int gLastTimeL = 0;
double gVR = 0;
double gVL = 0;
double gX = 0;
double gY = 0;
double gAngle = 0;
int gPrevEncStateR = 0;
int gPrevEncStateL = 0;
int gEncTicksR = 0;
int gEncTicksL = 0;
int gEncTicksOldR = 0;
int gEncTicksOldL = 0;
int gStartTime = 0;
int gDisplayState = 0;
long gThisTime = 0;
long gOldTime = 0;

int gDesiredSpeedL = 0;
int gDesiredSpeedR = 0;
int gCurrentSpeedL = 0;
int gCurrentSpeedR = 0;
const int gAcceleration = 1000; // set to 1000 to make obsolete

int gSpeedR1 = 120; //140
int gSpeedL1 = 120; //120 for straight
int gSpeedR2 = 120;
int gSpeedL2 = -120;
int gSpeedR3 = 0;
int gSpeedL3 = 0;

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

void loop() {

  long thisTime = millis();
  int thisSpeedR,thisSpeedL;
  
  if (thisTime-gStartTime <= 5000){
    thisSpeedR = gSpeedL1;
    thisSpeedL = gSpeedR1;
  }
  else if (thisTime-gStartTime <= 10000){
    thisSpeedR = gSpeedL2;
    thisSpeedL = gSpeedR2;
  }
  else if (thisTime-gStartTime <= 15000){
    thisSpeedR = gSpeedL3;
    thisSpeedL = gSpeedR3;
  }
  else{
    thisSpeedR = 0;
    thisSpeedL = 0;
  }
  gDesiredSpeedR = thisSpeedR;
  gDesiredSpeedL = thisSpeedL;

  // acceleration to avoid slipping
  if(gCurrentSpeedR<gDesiredSpeedR){
    gCurrentSpeedR = min(gCurrentSpeedR+gAcceleration,gDesiredSpeedR);
  }
  else if(gCurrentSpeedR>gDesiredSpeedR){
    gCurrentSpeedR = max(gCurrentSpeedR-gAcceleration,gDesiredSpeedR);
  }

  if(gCurrentSpeedL<gDesiredSpeedL){
    gCurrentSpeedL = min(gCurrentSpeedL+gAcceleration,gDesiredSpeedL);
  }
  else if(gCurrentSpeedL>gDesiredSpeedL){
    gCurrentSpeedL = max(gCurrentSpeedL-gAcceleration,gDesiredSpeedL);
  }

  // tell the motors to get busy
  drive(gCurrentSpeedR,gCurrentSpeedL);
  //delay
  delay(CYCLEDELAYTIME);

  gThisTime = millis();

  long deltaTime = gThisTime-gOldTime;

  // get encoder ticks
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
  displayValues[0] = gX; //mm
  displayValues[1] = gY; //mm
  displayValues[2] = gAngle*(180/PI); //deg

  displayOne(displayValues[gDisplayState],gDisplayState);

  gOldTime = gThisTime;

  /*
  Serial.print(displayValues[0]);
  Serial.print(" ");
  Serial.println(displayValues[1]);
  Serial.println(displayValues[2]);
  */
}
