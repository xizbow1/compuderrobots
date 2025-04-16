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

//function drive
void drive(int theta, int speed){
  double L = 23.5;
  double W = 16.25;
  double wheelBase = W/L;
  double rad = (theta * PI) / 180;
  double vR = speed*(1.0 + 0.5 *((wheelBase)/(tan(rad))));
  double vL = speed*(1.0 - 0.5 *((wheelBase)/(tan(rad))));
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
  
  //Straight
  if(theta == 90){
    steeringServo.write(theta);
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
void stop(){
  steeringServo.write(90);
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);
  analogWrite(pinENA12, 0);
  analogWrite(pinENA34, 0);
}

void loop()
{  
 
  //straight
  drive(90,128);//call drive with the angle and the speed
  delay(3000);
  
  //right turn
  drive(130,128);
  delay(3000);
    
  //straight
  drive(90,128);
  delay(3000);
  
  //left turn
  drive(50,128);
  delay(3000);
  
  //straight
  drive(90,128);
  delay(3000);
  
  //stop
  stop();
  delay(3000);
}
