#define DRIVER 5 // control speed of motor, pwm 0-255
#define DIRR1 7 // make output1 high or low, clockwise/counter
#define DIRR2 4 // output pin 2 high or low, needs to be opposite of input1
#define DRIVEL 6
#define DIRL1 2
#define DIRL2 3

void setupDrive(){
  //Set up the pins for driving
  pinMode(DRIVER,OUTPUT);
  pinMode(DIRR1,OUTPUT);
  pinMode(DIRR2,OUTPUT);
  pinMode(DRIVEL,OUTPUT);
  pinMode(DIRL1,OUTPUT);
  pinMode(DIRL2,OUTPUT);

  digitalWrite(DIRR1,HIGH);
  digitalWrite(DIRR2,LOW);
  digitalWrite(DIRL1,HIGH);
  digitalWrite(DIRL2,LOW);
}

void drive(int speedR, int speedL){
  //Sets motor speeds to ints given

  //Handle negative speeds
  if(speedR>=0){
    digitalWrite(DIRR1,LOW);
    digitalWrite(DIRR2,HIGH);
  }
  else{
    digitalWrite(DIRR1,HIGH);
    digitalWrite(DIRR2,LOW);
  }
  if(speedL>=0){
    digitalWrite(DIRL1,LOW);
    digitalWrite(DIRL2,HIGH);
  }
  else{
    digitalWrite(DIRL1,HIGH);
    digitalWrite(DIRL2,LOW);
  }
  
  //Set motor speeds.
  //Note that motors will stay at these speeds
  //  indefinitely
  analogWrite(DRIVEL, abs(speedL));
  analogWrite(DRIVER, abs(speedR));
}
