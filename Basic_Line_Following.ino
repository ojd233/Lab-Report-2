//Define the motor pins
#define PWMa 7
#define PWMb 5
#define PWMc 8
#define PWMd 6

//Define what pins the sensors are connected to
int pinleft = A0;
int pinright = A1;

//Define a variable to store the data that is read from the sensors
int valleft = 0;
int valright = 0;

void setup() {
  // configure the motor control pins as outputs
  pinMode(PWMa, OUTPUT);
  pinMode(PWMb, OUTPUT);
  pinMode(PWMc, OUTPUT);
  pinMode(PWMd, OUTPUT);
  //Begin a serial monitor for quick debugging
  Serial.begin(9600);


}


void loop(){
  //Set all the motors to off before the arduino retrieves data from the sensors
  digitalWrite(PWMa, LOW);
  digitalWrite(PWMb, LOW);
  digitalWrite(PWMc, LOW);
  digitalWrite(PWMd, LOW);

  //Delay for 5 ms
  delay(5);

  //Read values from the left and right sensors
  valleft = analogRead(pinleft);
  valright = analogRead(pinright);

  //Print the data to the serial monitor for debugging and finding the values for WHITE and BLACK surfaces
  Serial.println(valleft);
  Serial.println(valright);

  //Check if the analogue reading from the sensors fits one set of parameters for turning Left
  if ((valleft > 500) && (valright < 600)) {

      //Print a statement that can be used to tell if the EEE-Bot knows which way it should be turning
      Serial.println("HELLO A");
      //Set the motors to turn left (Comment this section out if using for debugging or finding sensor values)
      digitalWrite(PWMa, HIGH);
      digitalWrite(PWMb, LOW);
      digitalWrite(PWMc, HIGH);
      digitalWrite(PWMd, LOW);
      //Delay for 10ms
      delay(10);
      }
      
  //Check if the analogue reading from the sensors fits one set of parameters for turning Right
  else if ((valright > 600) && (valleft < 300)) {
      //Print a statement that can be used to tell if the EEE-Bot knows which way it should be turning
      Serial.println("HELLO B");
      //Set the motors to turn right (omment this section out if using for debugging or finding sensor values)
      digitalWrite(PWMa, LOW);
      digitalWrite(PWMb, HIGH);
      digitalWrite(PWMc, LOW);
      digitalWrite(PWMd, HIGH);
      //Delay for 10ms
      delay(10);
      
      }
      
  
  
 
//Else statement that tells the EEE-Bot to move in a straight line if neither of the above conditions are true
  else {
    //Print a debug statement, used to tell if the motors know to go straight ahead
    Serial.println("Forward");
    //Set the motors to go straight (Comment this section out if using for debugging or finding sensor values)
    digitalWrite(PWMa,LOW);
    digitalWrite(PWMb, HIGH);
    digitalWrite(PWMc, HIGH);
    digitalWrite(PWMd, LOW);
    //Delay for 10ms
    delay(10);
  }
  
  

}
