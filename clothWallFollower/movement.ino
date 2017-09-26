void moveDirection(float dir){
  
  Serial.print("v1 v2: ");
  Serial.print(vl);
  Serial.print(" ");
  Serial.print(vr);
  Serial.println();

  driveForward(vl,vr);
}

void driveForward(int speedL, int speedR){
  //Use analogWrite to send speedL to the driveL pin
  analogWrite(DRIVEL, speedL);
  //Use digitalWrite to send high and low signals
  // to dirL1 and dirL2 to set motor direction.
  digitalWrite(DIRL1, HIGH);
  digitalWrite(DIRL2, LOW);
  analogWrite(DRIVER, speedR);
  digitalWrite(DIRR1, HIGH);
  digitalWrite(DIRR2, LOW);
}
