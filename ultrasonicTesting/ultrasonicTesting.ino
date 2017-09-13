/* http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/
 * Used the above example as a reference.
 * Otherwise, written by Vaughn J. Bernard
 */

#define TRIGPIN 13
#define ECHOPIN 12
#define TRANSMITTIME 10 // in microseconds
#define DISTANCECONSTANT 1/2.9
#define CYCLEDELAYTIME 500

void setup() {
  Serial.begin(9600);
  pinMode(TRIGPIN,OUTPUT);
  pinMode(ECHOPIN,INPUT);
}

void loop() {
  // write to terminal
  Serial.print(getDistanceMM(TRIGPIN,ECHOPIN));
  Serial.println(" mm");
}
