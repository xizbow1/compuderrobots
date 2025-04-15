// C++ code
//
#include <Servo.h>

Servo steeringServo;
int portSpeed =9600;

//right motor
const int pinENA12 = 3;
const int pinIN1 = 4;
const int pinIN2 = 5;
//left motor
const int pinENA34 = 6;
const int pinIN3 = 7;
const int pinIN4 = 8;

void setup()
{
  //right motor
  pinMode(pinENA12, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  //left motor
  pinMode(pinENA34, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  //steering servo
  steeringServo.attach(9);
  
}

void turn(int theta){
  const int wheelBase = 12;
  double rad = (theta * PI) / 180;
  double vO = (1.0 + (1.0/2.0)*(wheelBase)*(tan(rad)));
  double vI = (1.0 - (1.0/2.0)*(wheelBase)*(tan(rad)));
  
  //Not Turning
  if(theta == 90){
    
    steeringServo.write(theta);
    
    vI = vO;
    digitalWrite(pinIN1, LOW);
  	digitalWrite(pinIN2, HIGH);
  	digitalWrite(pinIN3, HIGH);
  	digitalWrite(pinIN4, LOW);
  	analogWrite(pinENA12, vO);
  	analogWrite(pinENA34, vI);
    
  //Right turn
  }else if(theta > 90){
    
    steeringServo.write(theta);
    
  	digitalWrite(pinIN1, LOW);
  	digitalWrite(pinIN2, HIGH);
  	digitalWrite(pinIN3, HIGH);
  	digitalWrite(pinIN4, LOW);
    
  	analogWrite(pinENA12, vI);
  	analogWrite(pinENA34, vO);
    
  //Left Turn
  }else if(theta < 90){
    steeringServo.write(theta);
    
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
    digitalWrite(pinIN3, HIGH);
    digitalWrite(pinIN4, LOW);
    
    analogWrite(pinENA12, vO);
    analogWrite(pinENA34, vI);
  }
}

void loop()
{
  
  
  //straight
  turn(90);
  delay(3000);
  
  //right turn
  turn(130);
  delay(3000);
    
  //straight
  turn(90);
  delay(3000);
  
  //left turn
  turn(50);
  delay(3000);
  
  //straight
  turn(90);
  delay(3000);
  
  //stop
  steeringServo.write(90);
  analogWrite(pinENA12, 0);
  analogWrite(pinENA34, 0);
  delay(3000);
}
