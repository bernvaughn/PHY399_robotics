#define ENCX 8
#define ENCY 9

int gTimePassedX = 0;
int gTimePassedY = 0;
int gX = 0;
int gY = 0;
int gPrevEncX = 0;
int gPrevEncY = 0;

void setup() {
  setupDisplay();
  
  pinMode(ENCX,INPUT);
  pinMode(ENCY,INPUT);

  gPrevEncX = digitalRead(ENCX);
  gPrevEncY = digitalRead(ENCY);
  
}

void loop() {
  // for reference:
  // vel = (prev_Vr+prev_Vl)/2
  // angle = prev_angle + time * (prev_Vr-prev_Vl)/omega
  // x = prev_x + time * -vel * cos(angle)
  // y = prev_y + time * vel * sin(angle)

  //if within first 5 seconds
  //  move with first speeds
  //if within second 5 seconds
  //  move with second speeds
  //if within third 5 seconds
  //  move with third speeds

  //on r encoder state change
  //  update Vr based on time since last update
  //on l encoder state change
  //  update Vl based on time since last update
  
  //update x,y based on formulas above
  //display x,y
  //display orientation intermittently
  

}
