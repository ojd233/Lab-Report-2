#define PWMa 7
#define PWMb 5
#define PWMc 8
#define PWMd 6
int pinleft = A0;
int pinright = A1;
int valleft = 0;
int valright = 0;


// define a limit value
#define LIMIT 1000

void setup() {
  // configure the motor control pins as outputs
  pinMode(PWMa, OUTPUT);
  pinMode(PWMb, OUTPUT);
  pinMode(PWMc, OUTPUT);
  pinMode(PWMd, OUTPUT);
  Serial.begin(9600);


}
void setLeftMotor(int16_t motorSpeed){
  constrain(motorSpeed, -255, 255);

  if(motorSpeed < 0){
    digitalWrite(PWMc, HIGH);
    analogWrite(PWMd, 255- motorSpeed);
  }
  else{
    digitalWrite(PWMc, LOW);
    analogWrite(PWMd, abs(motorSpeed));
  }
}

void setRightMotor(int16_t motorSpeed){
  constrain(motorSpeed, -255, 255);

  if(motorSpeed >0){
    digitalWrite(PWMa, HIGH);
    analogWrite(PWMb, 255- motorSpeed);
  }
  else{
    digitalWrite(PWMa, LOW);
    analogWrite(PWMb, abs(motorSpeed));
  }
}


void loop(){
  digitalWrite(PWMa, LOW);
  digitalWrite(PWMb, LOW);
  digitalWrite(PWMc, LOW);
  digitalWrite(PWMd, LOW);
  delay(5);
  valleft = analogRead(pinleft);
  valright = analogRead(pinright);
  Serial.println(valleft);
  Serial.println(valright);
  if ((valleft > 500) && (valright < 600)) {
      
      Serial.println("HELLO A");
      digitalWrite(PWMa, HIGH);
      digitalWrite(PWMb, LOW);
      digitalWrite(PWMc, HIGH);
      digitalWrite(PWMd, LOW);
      delay(10);
      }
      
  
  else if ((valright > 600) && (valleft < 300)) {
      
      Serial.println("HELLO B");
      digitalWrite(PWMa, LOW);
      digitalWrite(PWMb, HIGH);
      digitalWrite(PWMc, LOW);
      digitalWrite(PWMd, HIGH);
      delay(10);
      
      }
      
  
  
 

  else {
    Serial.println("Forward");
    digitalWrite(PWMa,LOW);
    digitalWrite(PWMb, HIGH);
    digitalWrite(PWMc, HIGH);
    digitalWrite(PWMd, LOW);
    delay(10);
  }
  
  

}
