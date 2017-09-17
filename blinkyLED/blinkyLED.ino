#define LEDPIN 13
int gToBlink = 0;

void setup() {
  Serial.begin(9600);
  gToBlink = 2; // gToBlink*10 seconds of blinking
  pinMode(LEDPIN,OUTPUT);
}

void blinkLED(int pin){
  digitalWrite(pin,HIGH);
  delay(50);
  digitalWrite(pin,LOW);
  delay(50);
}

void loop() {
  while (millis() <= 10000*gToBlink){
    Serial.println(millis());
    blinkLED(LEDPIN);
  }
}
