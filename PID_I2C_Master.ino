//Include needed libraries
#include <Wire.h>
#include <LiquidCrystal.h> 

//define the sensor pins
int pinleftin = A0;
int pinrightin = A1;
int pinleftout = A2;
int pinrightout = A3;
float valleft1 = 0;
float valleft2 = 0;
float valright1 = 0;
float valright2 = 0;

//Define variables for calibration BLACK BACKGROUND
int valleft1B = 0;
int valleft2B = 0;
int valright1B = 0;
int valright2B = 0;+


//Define variables for calibration WHITE BACKGROUND
int valleft1W = 0;
int valleft2W = 0;
int valright1W = 0;
int valright2W = 0;

//define the distances from the reference point (centre of the car) USED FOR WEIGHTED AVERAGE FORMULA
float distlout = 1.3;
float distlin = 2.45;
float distrin = 4.05;
float distrout = 5.35;
float lineposition;

//define the variables for the error and the 3 PID components
float error;
float errorLast;
float Kp = 100;
float Ki = .25;
float Kd = 10;
int loopNum;
float errorSum;
float PID;
int leftMotorSpeed;
int rightMotorSpeed;

String message_in;

//Define LCD Pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//setup motors and LCD
void setup() {
  Wire.begin(0x08);
  Serial.begin(9600);
  pinMode(9, HIGH);
  pinMode(10, HIGH);
  lcd.begin(16, 2);
  lcd.home();
  

  //BLACK BACKGROUND - Calibration
  Serial.println("--Please place all sensors above a black background--");
  lcd.print("Calibrating");
  lcd.setCursor(1, 2);
  lcd.print("Black");
  digitalWrite(10, HIGH);
  delay(1000);
  valleft1B = analogRead(pinleftin);
  valright1B = analogRead(pinrightin);
  valleft2B = analogRead(pinleftout);
  valright2B = analogRead(pinrightout);
  delay(1000);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  delay(1000);
  lcd.clear();

  //WHITE BACKGROUND - Calibration
  Serial.println("--Please place all sensors above a white background--");
  lcd.print("Calibrating");
  lcd.setCursor(1,2);
  lcd.print("White");
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  delay(1000);
  valleft1W = analogRead(pinleftin);
  valright1W = analogRead(pinrightin);
  valleft2W = analogRead(pinleftout);
  valright2W = analogRead(pinrightout);
  delay(1000);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  lcd.clear();
  delay(2000);
  


}

//Weighted average calculation
float weightedAverage(float distlefto, float distlefti, float distrighti, float distrighto, float valleft2, float valleft1, float valright1, float valright2){ 
  float top, bottom, wa;
  top = (distlefto*valleft2)+(distlefti*valleft1)+(distrighti*valright1)+(distrighto*valright2);
  bottom = (valleft2+valleft1+valright1+valright2);
  wa = top/bottom;
  return wa;
}


//Recieve event function
void receiveEvent(){
  message_in = "";
  while(Wire.available()) {
    char c = Wire.read();
    message_in += c;
  }
}
//Request event function
void requestEvent(){
  if (message_in == "L"){
    Wire.write(leftMotorSpeed);
  }
  if (message_in == "R"){
    Wire.write(rightMotorSpeed);
  }
}
  
void loop(){
 
  
  //CALIBRATION SECTION
  valleft1 = analogRead(pinleftin);
  valright1 = analogRead(pinrightin);
  valleft2 = analogRead(pinleftout);
  valright2 = analogRead(pinrightout);
  //Constrain two left sensors
  valleft1 = constrain(valleft1, valleft1B, valleft1W);
  valleft2 = constrain(valleft2, valleft2B, valleft2W);
  valright1 = constrain(valright1, valright1B, valright1W);
  valright2 = constrain(valright1, valright1B, valright1W);

  //Map values for each sensor to new range
  valleft1 = map(valleft1, valleft1B, valleft1W, 0, 1023);
  valleft2 = map(valleft2, valleft2B, valleft2W, 0, 1023);
  valright1 = map(valright1, valright1B, valright1W, 0, 1023);
  valright2 = map(valright2, valright2B, valright2W, 0, 1023);



  //Send and recieve data based on PID values
  lineposition = weightedAverage(distlout,distlin,distrin,distrout,valleft2,valleft1,valright1,valright2);
  error = 3.3 - lineposition;
  if (errorSum == 0) {
     errorSum = error;
  }
  PID = (Kp * error) + (Ki * errorSum) + (Kd * (error - errorLast));
  errorLast = error;
  errorSum = errorSum + error;
  Serial.println(PID);

  //Calculate motor speeds, send and recieve data 
  leftMotorSpeed = 128 - PID;
  

  rightMotorSpeed = 128  - PID;

  
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  //Print PID value to LCD
  lcd.home();
  lcd.print("PID:");
  lcd.setCursor(0,1);
  lcd.print(PID);
  
  

  //Serial.println(leftMotorSpeed);
  //Serial.println(rightMotorSpeed);
  Serial.println("");
  

  




  
  

}
