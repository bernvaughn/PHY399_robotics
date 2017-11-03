#define ENCR 9
#define ENCL 8

#define CYCLEDELAYTIME 20 //ms

int gLastTimeR = 0;
int gLastTimeL = 0;
int gX = 0;
int gY = 0;
int gPrevEncStateR = 0;
int gPrevEncStateL = 0;
int gStartTime = 0;

int gSpeedR1 = 150;
int gSpeedL1 = 140;
int gSpeedR2 = 160;
int gSpeedL2 = 150;
int gSpeedR3 = 120;
int gSpeedL3 = 120;

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
    displayTwo(gSpeedR1,gSpeedL1);
  }
  else if (thisTime-gStartTime <= 10000){
    drive(gSpeedR2,gSpeedL2);
    displayTwo(gSpeedR2,gSpeedL2);
  }
  else if (thisTime-gStartTime <= 15000){
    drive(gSpeedR3,gSpeedL3);
    displayTwo(gSpeedR3,gSpeedL3);
  }
  else{
    drive(0,0);
    displayTwo(0,0);
  }


  //on r encoder state change
  //  update Vr based on time since last update
  //on l encoder state change
  //  update Vl based on time since last update

  //when the time ends with a certain number
  //update x,y based on formulas above

  
  //display x,y
  //display orientation intermittently
  
  delay(CYCLEDELAYTIME);
}
