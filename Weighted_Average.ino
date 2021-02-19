//Define the motor pins
#define PWMa 7
#define PWMb 5
#define PWMc 8
#define PWMd 6

//define the sensor pins
int pinleftin = A0;
int pinrightin = A1;
int pinleftout = A2;
int pinrightout = A3;

//Define the variables to be used to store the sensor data
int valleft1 = 0;
int valleft2 = 0;
int valright1 = 0;
int valright2 = 0;

//define the distances from the reference point (centre of the array)
float distlout = -2.1;
float distlin = -0.8;
float distrin = 0.8;
float distrout = 2.1;
float lineposition;



void setup() {
  // configure the motor control pins as outputs
  pinMode(PWMa, OUTPUT);
  pinMode(PWMb, OUTPUT);
  pinMode(PWMc, OUTPUT);
  pinMode(PWMd, OUTPUT);
  //Begin a serial connection for debugging
  Serial.begin(9600);

//allow for the speed of the motors to be changed

}


float weightedAverage(float distlefto, float distlefti, float distrighti, float distrighto, float valleft2, float valleft1, float valright1, float valright2){ 
  //define the storage and output variables
  float top, bottom, wa;
  //Calculate the numerator for the Weighthed average formula
  top = (distlefto*valleft2)+(distlefti*valleft1)+(distrighti*valright1)+(distrighto*valright2);
  //Calculate the denominator for the weighted average formula
  bottom = (valleft2+valleft1+valright1+valright2);
  //Divide the numerator by the denominator
  wa = top/bottom;
  //Return the value for the weighted average
  return wa;
}


void loop(){
  //Set all motors to LOW
  digitalWrite(PWMa, LOW);
  digitalWrite(PWMb, LOW);
  digitalWrite(PWMc, LOW);
  digitalWrite(PWMd, LOW);
  //Delay for 20ms
  delay(20);

  //Read data from the sensor pins
  valleft1 = analogRead(pinleftin);
  valright1 = analogRead(pinrightin);
  valleft2 = analogRead(pinleftout);
  valright2 = analogRead(pinrightout);
  //Print the recorded values to the serial monitor, mainly used for debugging and testing
  Serial.println(valleft2);
  Serial.println(valleft1);
  Serial.println(valright1);
  Serial.println(valright2);

  //Call the function for weighted average and input the sensor values, alongside each sensors distance from the central refernce point
  lineposition = weightedAverage(distlout,distlin,distrin,distrout,valleft2,valleft1,valright1,valright2);
  //Print the weighted average value (mainly used for testing and debugging purposes)
  Serial.println(lineposition);

  //Conditional statement for if the EEE-Bot is on the left side of the line
  if (lineposition < 0) { 
      //A print statement (Mainly for debugging)
      Serial.println("HELLO A");
      //Set the motors to turn to the right to adjust back onto the line
      digitalWrite(PWMa, HIGH);
      digitalWrite(PWMb, LOW);
      digitalWrite(PWMc, HIGH);
      digitalWrite(PWMd, LOW);
      //Delay 10ms
      delay(10);
      }
  //Conditional statement for if the EEE-Bot is on the right hand side of the line
  else if (lineposition > 0.2) {
      //A print statement mainly used for debugging
      Serial.println("HELLO B");
      //Set the motors to turn the vehicle left to adjust to the line
      digitalWrite(PWMa, LOW);
      digitalWrite(PWMb, HIGH);
      digitalWrite(PWMc, LOW);
      digitalWrite(PWMd, HIGH);
      //Delay 10ms
      delay(10);
      
      }
  //Conditional statement for if neither of the above events occur
  else if (lineposition <= 0.2 && lineposition >=0) {
    //A print statement mainly used for debugging
    Serial.println("Forward");
    //Set the motors to both turn on in the forarwd direction
    digitalWrite(PWMa, LOW);
    digitalWrite(PWMb, HIGH);
    digitalWrite(PWMc, HIGH);
    digitalWrite(PWMd, LOW);
    //Delay 20ms
    delay(20);
  }
  
  

}
