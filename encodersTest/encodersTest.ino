void setup() {
  Serial.begin(9600);
  pinMode(8,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  float val;
  val = digitalRead(8);
  Serial.println(val);
}
