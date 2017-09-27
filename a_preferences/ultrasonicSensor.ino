/*
 * Function to get distance data
 */
#define TRANSMITTIME 10 // in microseconds

void getTargetDistance(){
  // Uses gTargetDistance and gTargetDistanceCount
  float thisDistance = getDistance(TRIGPIN,ECHOPIN);
    
  if (gTargetDistanceCount == 0){
    // if there is no current target distance, set it.
    float testDistance = getDistance(TRIGPIN,ECHOPIN);
    if ((thisDistance/10 < testDistance) and (thisDistance*10 > testDistance)){
      // make sure the first distance isn't an anomally
      gTargetDistance=thisDistance;
      gTargetDistanceCount = 1;
    }
  }
  else if ((thisDistance/10 < gTargetDistance) and (thisDistance*10 > gTargetDistance)){
    // update gTargetDistance with new average if not anomally
    gTargetDistance*=gTargetDistanceCount;
    gTargetDistance+=thisDistance;
    gTargetDistance/=gTargetDistanceCount+1;
    gTargetDistanceCount+=1;
  }
}
 
int getDistance(int trigPin,int echoPin) {
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
    distance[i] = (duration/2);
  }

  return (distance[0]+distance[1])/2;
}

