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

int prevTheta = 0;

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
void drive(int theta, bool dir){
 prevTheta = theta;
  int speed = 64;
  double L = 23.5;
  double W = 16.25;
  double wheelBase = W/L;//calculate the wheel base
  double rad = (theta * PI) / 180;//Convert theta to radians
  
  //calculate right and left speed based on turn angle (theta)
  double vR = speed*(1.0 + 0.5 *((wheelBase)/(tan(rad))));
  double vL = speed*(1.0 - 0.5 *((wheelBase)/(tan(rad))));
  
  //Sending power to the motors
  if(dir == true){
  	digitalWrite(pinIN1, HIGH);
  	digitalWrite(pinIN2, LOW);
  	digitalWrite(pinIN3, HIGH);
  	digitalWrite(pinIN4, LOW);
  } else {
    digitalWrite(pinIN1, LOW);
  	digitalWrite(pinIN2, HIGH);
  	digitalWrite(pinIN3, LOW);
  	digitalWrite(pinIN4, HIGH);
  }
  
  //Straight
  if(theta == 90){
    if (prevTheta > 90){
      theta = 84;
    }
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
  if (prevTheta > 90){
    steeringServo.write(84);
    prevTheta = 84;
  }else{
    steeringServo.write(90);//set wheels straight
    prevTheta = 90;
  }
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
  steeringServo.write(90);//set wheels straight
  delay(1000);
  
  //figure 8 drive
  
  //right turn
  drive(160,true);
  delay(2000);
    
  //straight
  drive(90,true);
  delay(1000);
  
  //left turn
  drive(20,true);
  delay(2000);
  
  //straight
  drive(90,true);
  delay(1000);
  
  //stop
  stop();
  delay(3000);
  
  //figure 8 reverse
  
  //reverse
  drive(90,false);
  delay(1000);
  
  //left turn;
  drive(20,false);
  delay(2000);
  
  //reverse
  drive(90,false);
  delay(1000);
  
  //right turn;
  drive(160,false);
  delay(2000);
  
  //stop
  stop();
  delay(3000);
}
