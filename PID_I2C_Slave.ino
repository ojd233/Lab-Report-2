//Include I2C
#include <Wire.h>

//Define Motor Pins
#define PWMa 7
#define PWMb 5
#define PWMc 8
#define PWMd 6


void setup() {
  //Begin I2C Communication with the slave arduino
  Wire.begin(0x08);
  Serial.begin(9600);
  
  //Setup Motors
  pinMode(PWMa, OUTPUT);
  pinMode(PWMb, OUTPUT);
  pinMode(PWMc, OUTPUT);
  pinMode(PWMd, OUTPUT);
  
  delay(10000);
}


void loop(){
  //Set motors to idle state
  digitalWrite(PWMa, LOW);
  digitalWrite(PWMc, HIGH);
  //Run functions for left and right motors
  LEFT();
  RIGHT();
  
}
//Send and recieve data for left motor
void LEFT(){
  Wire.beginTransmission(0x08);
  //Send data to the master
  Wire.write("L");
  Serial.println("L");
  Wire.endTransmission();

  //Request data from the master
  Wire.requestFrom(0x08, 1);
  while(Wire.available()){
    int PIDL = Wire.read();
    //Print and set motor value
    Serial.println(PIDL);
    analogWrite(PWMb, PIDL);
  }
}
//Send and recieve data for right motor
void RIGHT(){
  Wire.beginTransmission(0x08);
  //Send data to master
  Wire.write("R");
  Wire.endTransmission();
  Serial.println("R");

  //Request data from master
  Wire.requestFrom(0x08, 1);
  while(Wire.available()){
    int PIDR = Wire.read();
    //Print and set motor value
    Serial.println(PIDR);
    analogWrite(PWMd, PIDR);
  }
}
