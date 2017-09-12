/*
 * Function to get distance data
 */
int getDistanceMM(int trigPin,int echoPin) {
  int transmitTime = 10 //in microseconds
  float distanceConstant = 1/2.9;
  float duration,distance;

  // 'clear' transmitter
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  // transmit
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(transmitTime);
  digitalWrite(trigPin,LOW);

  // calculate distance
  duration = pulseIn(echoPin,HIGH);
  distance = (duration/2) * distanceConstant;

  return distance;
}

