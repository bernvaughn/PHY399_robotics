#define TRIGPIN 10 // for ultrasonic
#define ECHOPIN 9 // for ultrasonic
#define LEDPIN 13 // on-board LED
#define SBUTTONPIN 8 // start button pin

#define DRIVER 6 // control speed of motor, pwm 0-255
#define DIRR1 2 // make output1 high or low, clockwise/counter
#define DIRR2 3 // output pin 2 high or low, needs to be opposite of input1
#define DRIVEL 5
#define DIRL1 7
#define DIRL2 4

#define KP 50
#define KI 10
#define KD 10

#define CYCLEDELAYTIME 10

float gTargetDistance = -1;
int gTargetDistanceCount = 0;

void driveForward(int time1,int speedL, int speedR){
  //Use analogWrite to send speedL to the driveL pin
  analogWrite(DRIVEL, speedL);
  //Use digitalWrite to send high and low signals
  // to dirL1 and dirL2 to set motor direction.
  digitalWrite(DIRL1, HIGH);
  digitalWrite(DIRL2, LOW);
  analogWrite(DRIVER, speedR);
  digitalWrite(DIRR1, HIGH);
  digitalWrite(DIRR2, LOW);
  delay(time1);
  analogWrite(DRIVEL, 0);
  analogWrite(DRIVER, 0);
}

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

void setup() {
  // setup pins
  pinMode(TRIGPIN,OUTPUT);
  pinMode(ECHOPIN,INPUT);
  pinMode(LEDPIN,OUTPUT);
  
  pinMode(DRIVER,OUTPUT);
  pinMode(DRIVEL,OUTPUT);
  pinMode(DIRR1,OUTPUT);
  pinMode(DIRR2,OUTPUT);
  pinMode(DIRL1,OUTPUT);
  pinMode(DIRL2,OUTPUT);

  Serial.begin(9600);

  // get target distance until connection is broken.
  // I can't get the button working. I don't know why.
  // I just disconnect a wire and this works.
  while(digitalRead(SBUTTONPIN)==HIGH){
    getTargetDistance();
    Serial.println(gTargetDistance);
  }

}

float oldDist = getDistance(TRIGPIN,ECHOPIN);
float sumDist = oldDist;
void loop() {
  float dist = getDistance(TRIGPIN,ECHOPIN);
  sumDist+=dist;
  float pid = KP*dist + KI*sumDist + KD*(dist-oldDist);
  driveForward(2000,255,255);
  oldDist = dist;

}
