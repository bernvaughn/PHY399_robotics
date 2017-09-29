

// global variables
float gTargetDistance = -1;
int gTargetDistanceCount = 0;

void setup() {
  // setup pins
  pinMode(TRIGPIN,OUTPUT);
  pinMode(ECHOPIN,INPUT);
  pinMode(LEDPIN,OUTPUT);
  pinMode(SBUTTONPIN,INPUT_PULLUP);
  
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
  while(digitalRead(SBUTTONPIN)){
    getTargetDistance();
    Serial.println(gTargetDistance);
  }
  // the button must be released before the loop() starts
  while(not digitalRead(SBUTTONPIN)){
    continue;
  }

}

// global variables for loop
float oldDist = gTargetDistance-getDistance(TRIGPIN,ECHOPIN);
float sumDist = oldDist;
int sw = 0;

void loop() {
  if(sw==0){
    // pid
    float dist = gTargetDistance-getDistance(TRIGPIN,ECHOPIN);
    sumDist+=dist;
    if(floor(dist/10)==0){
      sumDist = 0;
    }
    
    float pid = KP*dist + KI*sumDist + KD*(dist-oldDist); //pid

    if (dist<=0){
      driveForward(200,max(200-abs(pid),0));
    }
    else{
      driveForward(max(200-abs(pid),0),200);
    }

    oldDist = dist;
    
    delay(CYCLEDELAYTIME);
    
    // exit loop if button is pressed
    if (not digitalRead(SBUTTONPIN)){
      sw=1;
    }

    // serial interface
    Serial.print("dist oldDist sumDist ");
    Serial.print(dist);
    Serial.print(" ");
    Serial.print(oldDist);
    Serial.print(" ");
    Serial.print(sumDist);
    Serial.println();
    Serial.print("PID: ");
    Serial.println(pid);
  }
  else{
    // stop
    driveForward(0,0);
  }
  

}
