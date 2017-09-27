

// global variables
float gTargetDistance = -1;
int gTargetDistanceCount = 0;

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

  digitalWrite(DIRR1, HIGH);
  digitalWrite(DIRR2, LOW);
  digitalWrite(DIRL1, HIGH);
  digitalWrite(DIRL2, LOW);

  Serial.begin(9600);

  // get target distance until connection is broken.
  // I can't get the button working. I don't know why.
  // I just disconnect a wire and this works.
  while(digitalRead(SBUTTONPIN)==HIGH){
    getTargetDistance();
    Serial.println(gTargetDistance);
  }

}

// global variables for loop
float oldDist = getDistance(TRIGPIN,ECHOPIN);
float sumDist = oldDist;

void loop() {
  float dist = getDistance(TRIGPIN,ECHOPIN);
  sumDist+=dist;
  float pid = KP*dist + KI*sumDist + KD*(dist-oldDist);

  driveForward(150,150);
  /*driveForward(50,50+((gTargetDistance-dist)));

  Serial.print(50);
  Serial.print(" ");
  Serial.println(50+((gTargetDistance-dist)));*/

  // pseudocode
  /*
  if the distance is getting lower, increase power to close wheel
  if the distance is getting higher, increase power to far wheel

  need to take in to account the angle?
  */

  // serial interface
  /*
  Serial.print("dist oldDist sumDist ");
  Serial.print(dist);
  Serial.print(" ");
  Serial.print(oldDist);
  Serial.print(" ");
  Serial.print(sumDist);
  Serial.println();
  */

  oldDist = dist;

}
