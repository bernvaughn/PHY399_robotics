#define DELAYTIMEDISP 0
#define DIN 13
#define CS 12
#define CLK 11

void output(byte address, byte data){
  digitalWrite(CS,LOW);
  shiftOut(DIN,CLK,MSBFIRST,address); // High bit first
  shiftOut(DIN,CLK,MSBFIRST,data);
  digitalWrite(CS,HIGH);
}

void displayTwo(float x, float y){
  //displays two sets of floats with one decimal of precision.
  //Range: 000.0 to 999.9

  int digits[4];
  digits[0] = floor(x/100);
  digits[1] = floor((x-(digits[0]*100))/10);
  digits[2] = floor((x-digits[0]*100-digits[1]*10));
  digits[3] = floor((x-digits[0]*100-digits[1]*10-digits[2])*10);

  digits[2] += 128; // add decimal

  for(int i=8;i>=5;i--){
    output(i,digits[8-i]);
  }

  digits[0] = floor(y/100);
  digits[1] = floor((y-(digits[0]*100))/10);
  digits[2] = floor((y-digits[0]*100-digits[1]*10));
  digits[3] = floor((y-digits[0]*100-digits[1]*10-digits[2])*10);

  digits[2] += 128; // add decimal

  for(int i=4;i>=1;i--){
    output(i,digits[4-i]);
  }

   delay(DELAYTIMEDISP);
}

void setupDisplay() {
  digitalWrite(CS,HIGH);
  pinMode(DIN,OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(CLK,OUTPUT);

  output(0x0f, 0x00); //display test register - test mode off
  output(0x0c, 0x01); //shutdown register - normal operation
  output(0x0b, 0x07); //scan limit register - display digits 0 thru 7
  output(0x0a, 0x05); //intensity register - max brightness 00 to 0f
  output(0x09, 0xff); //decode mode register - CodeB decode all digits
}
