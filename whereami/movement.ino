#define DRIVER 6 // control speed of motor, pwm 0-255
#define DIRR1 2 // make output1 high or low, clockwise/counter
#define DIRR2 3 // output pin 2 high or low, needs to be opposite of input1
#define DRIVEL 5
#define DIRL1 7
#define DIRL2 4

void driveForward(int speedL, int speedR){
  //Use analogWrite to send speedL to the driveL pin
  analogWrite(DRIVEL, speedL);
  //Use digitalWrite to send high and low signals
  // to dirL1 and dirL2 to set motor direction.
  analogWrite(DRIVER, speedR);
  Serial.print("Speed(l,r): ");
  Serial.print(speedL);
  Serial.print(" ");
  Serial.println(speedR);
}
