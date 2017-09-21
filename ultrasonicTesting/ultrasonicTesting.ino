/* http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/
 * Used the above example as a reference.
 * Otherwise, written by Vaughn J. Bernard
 */

#define TRIGPIN 10
#define ECHOPIN 12
#define LEDPIN 13
#define CYCLEDELAYTIME 500

void setup() {
  Serial.begin(9600);
  pinMode(TRIGPIN,OUTPUT);
  pinMode(ECHOPIN,INPUT);
  pinMode(LEDPIN,OUTPUT);
}

void loop() {
  float dist = getDistanceMM(TRIGPIN,ECHOPIN);
  // write to terminal
  Serial.print(dist);
  Serial.println(" mm");

  if (dist <= 200){
    digitalWrite(LEDPIN,HIGH);
  }
  else{
    digitalWrite(LEDPIN,LOW);
  }
  
  delay(CYCLEDELAYTIME);
}
