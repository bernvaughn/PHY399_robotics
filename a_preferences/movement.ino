void driveForward(int speedL, int speedR){
  //Use analogWrite to send speedL to the driveL pin
  analogWrite(DRIVEL, speedL);
  //Use digitalWrite to send high and low signals
  // to dirL1 and dirL2 to set motor direction.
  analogWrite(DRIVER, speedR);
  delay(CYCLEDELAYTIME);
}
