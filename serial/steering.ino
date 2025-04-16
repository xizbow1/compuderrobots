// C++ code
//
#include <Servo.h>

Servo steeringServo;
int leftSpeed = 0;
int rightSpeed = 0;

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

//function drive()
//Takes 1 parameter integer
//returns nothing
void drive(int theta){
  int speed = 128;
  double L = 23.5;
  double W = 16.25;
  double wheelBase = W/L;//calculate the wheel base
  double rad = (theta * PI) / 180;//Convert theta to radians
  
  //calculate right and left speed based on turn angle (theta)
  double vR = speed*(1.0 + 0.5 *((wheelBase)/(tan(rad))));
  double vL = speed*(1.0 - 0.5 *((wheelBase)/(tan(rad))));
  
  //Sending power to the motors
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
  
  //Straight
  if(theta == 90){
    steeringServo.write(theta);//set turning angle to theta
    vR = speed;
    vL = speed;
    delay(500);
  	analogWrite(pinENA12, vR);
  	analogWrite(pinENA34, vL);
    
  //Turn
  }else{
    steeringServo.write(theta);
	delay(500);    
  	analogWrite(pinENA12, vR);
  	analogWrite(pinENA34, vL);
  }
}

//Function drive reverse
//Takes 1 parameter Integer
//returns nothing
void driveRev(int theta){
    int speed = 128;
  double L = 23.5;
  double W = 16.25;
  double wheelBase = W/L;//calculate the wheel base
  double rad = (theta * PI) / 180;//Convert theta to radians
  
  //calculate right and left speed based on turn angle (theta)
  double vR = speed*(1.0 + 0.5 *((wheelBase)/(tan(rad))));
  double vL = speed*(1.0 - 0.5 *((wheelBase)/(tan(rad))));
  
  //Sending power to the motors
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, HIGH);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, HIGH);
  
  //Straight
  if(theta == 90){
    steeringServo.write(theta);//set turning angle to theta
    vR = speed;
    vL = speed;
    delay(500);
  	analogWrite(pinENA12, vR);
  	analogWrite(pinENA34, vL);
    
  //Turn
  }else{
    steeringServo.write(theta);
	delay(500);    
  	analogWrite(pinENA12, vR);
  	analogWrite(pinENA34, vL);
  }
}

//function stop()
//no parameters
//returns nothing
void stop(){
  steeringServo.write(90);//set wheels straight
  //stop power to wheels
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);
  //set drive speed to 0
  analogWrite(pinENA12, 0);
  analogWrite(pinENA34, 0);
}

void loop()
{  
  //figure 8 drive
  
  //right turn
  drive(160);
  delay(2000);
    
  //straight
  drive(90);
  delay(1000);
  
  //left turn
  drive(20);
  delay(2000);
  
  //straight
  drive(90);
  delay(1000);
  
  //stop
  stop();
  delay(3000);
  
  //figure 8 reverse
  
  //reverse
  driveRev(90);
  delay(1000);
  
  //left turn;
  driveRev(160);
  delay(2000);
  
  //reverse
  driveRev(90);
  delay(1000);
  
  //right turn;
  driveRev(20);
  delay(2000);
  
  //stop
  stop();
  delay(3000);
}
