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
int gStartTime = 0;

int gSpeedR1 = 250;
int gSpeedL1 = 240;
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

  // TODO: handle integer overflow
  long thisTime = millis();
  // TODO: Properly handle negative speeds
  // TODO: Implement PID
  if (thisTime-gStartTime <= 5000){
    drive(gSpeedR1,gSpeedL1);
  }
  else if (thisTime-gStartTime <= 10000){
    drive(gSpeedR2,gSpeedL2);
  }
  else if (thisTime-gStartTime <= 15000){
    drive(gSpeedR3,gSpeedL3);
  }
  else{
    drive(0,0);
  }

  int thisEncStateR = digitalRead(ENCR);
  int thisEncStateL = digitalRead(ENCL);
  if ((gPrevEncStateR != thisEncStateR)){
    // each encoder wheel has 20 slots.
    // averaged wheel diameter: 63.64mm, radius = 31.82mm
    // circumference = 199.93mm
    //gVR = rate/time : time is thisTime - gLastTimeR
    if (thisEncStateR == 1){
      float t = (thisTime - gLastTimeR)*.001; //s
      float r = WHEELRADIUS*WHEELRADIUS*3.14/20; //mm
      gVR = r/t; //mm/s
      gLastTimeR = thisTime;
    }
    gPrevEncStateR = thisEncStateR;
  }
  if ((gPrevEncStateL != thisEncStateL)){
    if (thisEncStateL == 1){
      float t = (thisTime - gLastTimeL)*.001; //s
      float r = WHEELRADIUS*WHEELRADIUS*3.14/20; //mm
      gVL = r/t; //mm/s
      gLastTimeL = thisTime;
    }
    gPrevEncStateL = thisEncStateL;
  }

  float vel = ((gVR+gVL)/2); //mm/s
  float omega = ((gVR-gVL)/ROBOTWIDTH); //rot/s?

  float k00 = vel*cos(gAngle);
  float k01 = vel*sin(gAngle);
  float k02 = omega;

  float k10 = vel*cos(gAngle+(CYCLEDELAYTIME*k02)/2);
  float k11 = vel*sin(gAngle+(CYCLEDELAYTIME*k02)/2);
  float k12 = omega;

  float k20 = vel*cos(gAngle+(CYCLEDELAYTIME*k12)/2);
  float k21 = vel*sin(gAngle+(CYCLEDELAYTIME*k12)/2);
  float k22 = omega;

  float k30 = vel*cos(gAngle+(CYCLEDELAYTIME*k22));
  float k31 = vel*sin(gAngle+(CYCLEDELAYTIME*k22));
  float k32 = omega;

  gX = gX + (CYCLEDELAYTIME/6)*(k00+2*(k10+k20)+k30);
  gY = gY + (CYCLEDELAYTIME/6)*(k01+2*(k11+k21)+k31);
  gAngle = gAngle + (CYCLEDELAYTIME/6)*(k02+2*(k12+k22)+k32);

  //old:
  //gAngle = gAngle + (CYCLEDELAYTIME*.001) * (gVR-gVL)/ROBOTWIDTH;
  //gX = gX + (CYCLEDELAYTIME*.001) * -vel * cos(gAngle); //mm
  //gY = gY + (CYCLEDELAYTIME*.001) * vel * sin(gAngle); //mm

  displayTwo(gX,gY);
  Serial.print(gX);
  Serial.print(" ");
  Serial.println(gY);
  Serial.println(gAngle);
  // TODO: display orientation intermittently
  
  
  delay(CYCLEDELAYTIME);
}
