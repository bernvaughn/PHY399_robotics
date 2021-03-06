/*
 * Function to get distance data
 */
#define TRANSMITTIME 10 // in microseconds
#define DISTANCECONSTANT 1/2.9
 
int getDistanceMM(int trigPin,int echoPin) {
  //In: trigPin as where the ultrasonic sensor's trigger pin is wired to the arduino.
  //In: echoPin as where the ultrasonic sensor's echo pin is wired to the arduino.
  // NOTE:  trigPin must be set to WRITE
  //        echoPin must be set to READ
  //Out: Distance seen by the ultrasonic sensor in mm.

  //Variables
  float duration;
  float distance[1];

  //Takes two measurements and returns the average. This is a barbaric
  // way to handle some noisy data, and I want to do it in a smarter
  // way in the future.
  for (int i=0;i<2 ;i++)
  {
    // 'clear' transmitter
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
  
    // transmit
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(TRANSMITTIME);
    digitalWrite(trigPin,LOW);
  
    // calculate distance
    duration = pulseIn(echoPin,HIGH);
    distance[i] = (duration/2) * DISTANCECONSTANT;
  }

  return (distance[0]+distance[1])/2;
}

