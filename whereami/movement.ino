#define DRIVER 6 // control speed of motor, pwm 0-255
#define DIRR1 2 // make output1 high or low, clockwise/counter
#define DIRR2 3 // output pin 2 high or low, needs to be opposite of input1
#define DRIVEL 5
#define DIRL1 7
#define DIRL2 4

void setupDrive(){
  pinMode(DRIVER,OUTPUT);
  pinMode(DIRR1,OUTPUT);
  pinMode(DIRR2,OUTPUT);
  pinMode(DRIVEL,OUTPUT);
  pinMode(DIRL1,OUTPUT);
  pinMode(DIRL2,OUTPUT);

  digitalWrite(DIRR1,LOW);
  digitalWrite(DIRR2,HIGH);
  digitalWrite(DIRL1,LOW);
  digitalWrite(DIRL2,HIGH);
}

void drive(int speedR, int speedL){

  //TODO: handle negative speeds
  
  //Use analogWrite to send speedL to the driveL pin
  analogWrite(DRIVEL, speedL);
  //Use digitalWrite to send high and low signals
  // to dirL1 and dirL2 to set motor direction.
  analogWrite(DRIVER, speedR);
  //Serial.print("Speed(l,r): ");
  //Serial.print(speedL);
  //Serial.print(" ");
  //Serial.println(speedR);
}
