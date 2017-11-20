#define ENCR 9
#define ENCL 8

#define CYCLEDELAYTIME 15 //ms
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

int gSpeedR1 = 200;
int gSpeedL1 = 250;
int gSpeedR2 = 0;
int gSpeedL2 = 0;
int gSpeedR3 = 0;
int gSpeedL3 = 0;

void setup() {
  Serial.begin(9600);
  
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
  drive(thisSpeedR,thisSpeedL);

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
  gVR = gVR*(3.14159/10); //rads/s?
  gVR = gVR*WHEELRADIUS; //mm/s

  gVL = (gEncTicksL-gEncTicksOldL)/(CYCLEDELAYTIME*.001);
  gVL = gVL*(3.14159/10); //rads/s?
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
  
  if(millis()%5000==0){
    // display angle
    displayOne(gAngle*(180/PI),0); //deg
  }
  else if(millis()%2500==0){
    // display x
    displayOne(gX,1); //mm
  }
  else if(millis()%1000==0){
    // display y
    displayOne(gY,2); //mm
  }
  Serial.print(gX); //mm
  Serial.print(" ");
  Serial.println(gY); //mm
  Serial.println(gAngle); //radians
  
  
  delay(CYCLEDELAYTIME);
}
