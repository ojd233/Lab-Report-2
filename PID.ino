#define PWMa 7
#define PWMb 5
#define PWMc 8
#define PWMd 6

//define the sensor pins and data storage variables
int pinleftin = A0;
int pinrightin = A1;
int pinleftout = A2;
int pinrightout = A3;
int valleft1 = 0;
int valleft2 = 0;
int valright1 = 0;
int valright2 = 0;

//define the distances from the reference point (left of the car) USED FOR WEIGHTED AVERAGE FORMULA
float distlout = 1.3;
float distlin = 2.45;
float distrin = 4.05;
float distrout = 5.35;
float lineposition;

//define the variables for the error and the 3 PID components as well as other PID based variables
float error;
float errorLast;
float Kp = 100;
float Ki = .25;
float Kd = 10;
float errorSum;
float PID;
float leftMotorSpeed;
float rightMotorSpeed;



void setup() {
  // configure the motor control pins as outputs
  pinMode(PWMa, OUTPUT);
  pinMode(PWMb, OUTPUT);
  pinMode(PWMc, OUTPUT);
  pinMode(PWMd, OUTPUT);
  Serial.begin(9600);
}


float weightedAverage(float distlefto, float distlefti, float distrighti, float distrighto, float valleft2, float valleft1, float valright1, float valright2){ 
  float top, bottom, wa;
  top = (distlefto*valleft2)+(distlefti*valleft1)+(distrighti*valright1)+(distrighto*valright2);
  bottom = (valleft2+valleft1+valright1+valright2);
  wa = top/bottom;
  return wa;
}
  
void loop(){
  //Set the motors to their on values (PWMa is off as it will reverse the motor)
  digitalWrite(PWMa, LOW);
  digitalWrite(PWMc, HIGH);
  //Read values from the four sensors
  valleft1 = analogRead(pinleftin);
  valright1 = analogRead(pinrightin);
  valleft2 = analogRead(pinleftout);
  valright2 = analogRead(pinrightout);
  //Print the sensor values for debugging
  Serial.println(valleft2);
  Serial.println(valleft1);
  Serial.println(valright1);
  Serial.println(valright2);
  //Calculate the weighted avergae value using the corresponding function
  lineposition = weightedAverage(distlout,distlin,distrin,distrout,valleft2,valleft1,valright1,valright2);
  //Define the error value using the weighted average value
  error = 3.3 - lineposition;
  //Check if the error sum is equal to zero, if it is set the error sum to the error value
  if (errorSum == 0) {
     errorSum = error;
  }
  //Carry out the PID calculation using the defined constants and the values for error, errorlast and error sum. 
  PID = (Kp * error) + (Ki * errorSum) + (Kd * (error - errorLast));

  //Set the values for errorLast and errorSum to their next values
  errorLast = error;
  errorSum = errorSum + error;
  //Output the PID to the serial monitor for debugging
  Serial.println(PID);

  //Set the Left motor speed to the corect value using the base speed and the PID value
  leftMotorSpeed = 128 - PID;
  analogWrite(PWMb, leftMotorSpeed);
  //Print both the motor speed and a debugging phrase to the serial mointor for debugging
  Serial.println("LEFT");
  Serial.println(leftMotorSpeed);
  
  //Set the Right motor speed to the corresponding value using the base speed and the PID value
  rightMotorSpeed = 128 + PID;
  analogWrite(PWMd, rightMotorSpeed);
  //Print the motor speed and the debugging phrase to the serial mointor for debugging
  Serial.println("RIGHT");
  Serial.println(rightMotorSpeed);
}
