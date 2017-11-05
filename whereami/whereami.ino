#define ENCR 9
#define ENCL 8

#define CYCLEDELAYTIME 1 //ms

int gLastTimeR = 0;
int gLastTimeL = 0;
float gVR = 0;
float gVL = 0;
float gX = 0;
float gY = 0;
int gPrevEncStateR = 0;
int gPrevEncStateL = 0;
int gStartTime = 0;

int gSpeedR1 = 250;
int gSpeedL1 = 240;
int gSpeedR2 = 230;
int gSpeedL2 = 250;
int gSpeedR3 = 220;
int gSpeedL3 = 220;

void setup() {
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
  // angle = prev_angle + time * (prev_Vr-prev_Vl)/omega
  // x = prev_x + time * -vel * cos(angle)
  // y = prev_y + time * vel * sin(angle)

  // TODO: handle integer overflow
  int thisTime = millis();
  // TODO: Properly handle negative speeds
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
      float r = 199.93 / 20; //mm
      gVR = r/t; //mm/s
      gLastTimeR = thisTime;
    }
    gPrevEncStateR = thisEncStateR;
  }
  if ((gPrevEncStateL != thisEncStateL)){
    if (thisEncStateL == 1){
      float t = (thisTime - gLastTimeL)*.001; //s
      float r = 199.93 / 20; //mm
      gVL = r/t; //mm/s
      gLastTimeL = thisTime;
    }
    gPrevEncStateL = thisEncStateL;
  }
  displayTwo(gVR,gVL);

  //when the time ends with a certain number
  //update x,y based on formulas above

  
  //display x,y
  //display orientation intermittently
  
  delay(CYCLEDELAYTIME);
}
