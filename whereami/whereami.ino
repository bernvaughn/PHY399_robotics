#define ENCR 9
#define ENCL 8

#define CYCLEDELAYTIME 20 //ms
#define ROBOTWIDTH 120.57 //mm, +- 25.8mm
#define WHEELRADIUS 31.82 //mm

int gLastTimeR = 0;
int gLastTimeL = 0;
float gVR = 0;
float gVL = 0;
float gX = 0;
float gY = 0;
float gAngle = 0;
int gPrevEncStateR = 0;
int gPrevEncStateL = 0;
int gEncTicksR = 0;
int gEncTicksL = 0;
int gEncTicksOldR = 0;
int gEncTicksOldL = 0;
int gStartTime = 0;
int gDisplayState = 0;

int gDesiredSpeedL = 0;
int gDesiredSpeedR = 0;
int gCurrentSpeedL = 0;
int gCurrentSpeedR = 0;
const int gAcceleration = 10;

int gSpeedR1 = 100;
int gSpeedL1 = 100;
int gSpeedR2 = 120;
int gSpeedL2 = 80;
int gSpeedR3 = -100;
int gSpeedL3 = 100;

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
}

void loop() {
  // for reference:
  // vel = (prev_Vr+prev_Vl)/2
  // angle = prev_angle + time * (prev_Vr-prev_Vl)/width
  // x = prev_x + time * -vel * cos(angle)
  // y = prev_y + time * vel * sin(angle)

  long thisTime = millis();
  int thisSpeedR,thisSpeedL;
  // TODO: Properly handle negative speeds (in movement tab)
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


  gVR = (gEncTicksR-gEncTicksOldR)/(CYCLEDELAYTIME*.001);
  gVR = gVR*(PI/10); //rads/s?
  gVR = gVR*WHEELRADIUS; //mm/s

  gVL = (gEncTicksL-gEncTicksOldL)/(CYCLEDELAYTIME*.001);
  gVL = gVL*(PI/10); //rads/s?
  gVL = gVL*WHEELRADIUS; //mm/s
  

  float vel = ((gVR+gVL)/2); //mm/s
  float omega = ((gVR-gVL)/ROBOTWIDTH); //rot/s?

  float k00 = vel*cos(gAngle);
  float k01 = vel*sin(gAngle);
  float k02 = omega;

  float k10 = vel*cos(gAngle+(CYCLEDELAYTIME*.001*k02)/2);
  float k11 = vel*sin(gAngle+(CYCLEDELAYTIME*.001*k02)/2);
  float k12 = omega;

  float k20 = vel*cos(gAngle+(CYCLEDELAYTIME*.001*k12)/2);
  float k21 = vel*sin(gAngle+(CYCLEDELAYTIME*.001*k12)/2);
  float k22 = omega;

  float k30 = vel*cos(gAngle+(CYCLEDELAYTIME*.001*k22));
  float k31 = vel*sin(gAngle+(CYCLEDELAYTIME*.001*k22));
  float k32 = omega;

  gX = gX + ((CYCLEDELAYTIME*.001)/6)*(k00+2*(k10+k20)+k30); //mm
  gY = gY + ((CYCLEDELAYTIME*.001)/6)*(k01+2*(k11+k21)+k31); //mm
  gAngle = gAngle + ((CYCLEDELAYTIME*.001)/6)*(k02+2*(k12+k22)+k32); //radians

  gEncTicksOldR = gEncTicksR;
  gEncTicksOldL = gEncTicksL;

  if(millis()%1000<=2){
    gDisplayState+=1;
    if(gDisplayState>=3){
      gDisplayState=0;
    }
  }

  float displayValues[3];
  displayValues[0] = gX; //mm
  displayValues[1] = gY; //mm
  displayValues[2] = gAngle*(180/PI); //deg

  displayOne(displayValues[gDisplayState],gDisplayState);

  /*
  Serial.print(displayValues[0]);
  Serial.print(" ");
  Serial.println(displayValues[1]);
  Serial.println(displayValues[2]);
  */
  
  
  delay(CYCLEDELAYTIME);
}
