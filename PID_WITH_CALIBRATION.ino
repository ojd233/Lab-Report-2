#define PWMa 7
#define PWMb 5
#define PWMc 8
#define PWMd 6

#define REDLED 10
#define GREENLED 9

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
int valright2B = 0;


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
float Kp = 105;
float Ki = 10000;
float Kd = 15;
int loopNum;
float errorSum;
float PID;
float leftMotorSpeed;
float rightMotorSpeed;



//setup motors and calibration LEDs
void setup() {
  // configure the motor control pins as outputs
  pinMode(PWMa, OUTPUT);
  pinMode(PWMb, OUTPUT);
  pinMode(PWMc, OUTPUT);
  pinMode(PWMd, OUTPUT);

  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  Serial.begin(9600);


  //BLACK BACKGROUND - Calibration
  Serial.println("--Please place all sensors above a black background--");
  digitalWrite(REDLED, HIGH);
  delay(1000);
  valleft1B = analogRead(pinleftin);
  valright1B = analogRead(pinrightin);
  valleft2B = analogRead(pinleftout);
  valright2B = analogRead(pinrightout);
  delay(1000);
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, HIGH);
  delay(1000);

  //WHITE BACKGROUND - Calibration
  Serial.println("--Please place all sensors above a white background--");
  digitalWrite(REDLED, HIGH);
  digitalWrite(GREENLED, LOW);
  delay(1000);
  valleft1W = analogRead(pinleftin);
  valright1W = analogRead(pinrightin);
  valleft2W = analogRead(pinleftout);
  valright2W = analogRead(pinrightout);
  delay(1000);
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, HIGH);
  delay(1000);
  digitalWrite(GREENLED, LOW);

  delay(2000);



}


float weightedAverage(float distlefto, float distlefti, float distrighti, float distrighto, float valleft2, float valleft1, float valright1, float valright2){ 
  float top, bottom, wa;
  top = (distlefto*valleft2)+(distlefti*valleft1)+(distrighti*valright1)+(distrighto*valright2);
  bottom = (valleft2+valleft1+valright1+valright2);
  wa = top/bottom;
  return wa;
}


  
void loop(){
  digitalWrite(PWMa, LOW);
  digitalWrite(PWMc, HIGH);

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



  //MOVEMENT BASED ON CALIBRATED VALUES
  lineposition = weightedAverage(distlout,distlin,distrin,distrout,valleft2,valleft1,valright1,valright2);
  error = 3.3 - lineposition;
  if (errorSum == 0) {
     errorSum = error;
  }
  PID = (Kp * error) + (Ki * errorSum) + (Kd * (error - errorLast));
  errorLast = error;
  errorSum = errorSum + error;
  Serial.println(PID);

  leftMotorSpeed = 128 - PID;
  analogWrite(PWMb, leftMotorSpeed);
  Serial.println("RIGHT");
  Serial.println(leftMotorSpeed);

  rightMotorSpeed = 128  + PID;
  analogWrite(PWMd, leftMotorSpeed);
  Serial.println("LEFT");
  Serial.println(rightMotorSpeed);

  




  
  

}
