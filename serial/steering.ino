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

int prevAngle = 0;
int currSpd = 0;
bool currDir;

String cmdRec = "";
String cmd = "";
String value = "";
int intValue = 0;
int portSpeed = 9600;

void setup()
{
  //Serial port setup
  Serial.begin(portSpeed);
 
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
void drive(int theta, bool dir, int spd){

  if(theta < 25){
    theta = 25;
  } else if(theta > 155){
    theta = 155;
  }
  if(spd > 128){
    spd = 128;
  }
  currSpd = spd;

  double L = 23.5;
  double W = 16.25;
  double wheelBase = W/L;//calculate the wheel base
  double rad = (theta * PI) / 180;//Convert theta to radians
  
  //calculate right and left speed based on turn angle (theta)
  double vR = spd*(1.0 + 0.5 *((wheelBase)/(tan(rad))));
  double vL = spd*(1.0 - 0.5 *((wheelBase)/(tan(rad))));
  
  //Sending power to the motors
  if(dir == true){
    currDir = dir;
  	digitalWrite(pinIN1, HIGH);
  	digitalWrite(pinIN2, LOW);
  	digitalWrite(pinIN3, LOW);
  	digitalWrite(pinIN4, HIGH);
  } else {
    currDir = dir;
        digitalWrite(pinIN1, LOW);
  	digitalWrite(pinIN2, HIGH);
  	digitalWrite(pinIN3, HIGH);
  	digitalWrite(pinIN4, LOW);
  }
  
  //Straight
  if(theta == 90){
    if(prevAngle > 90){
      theta = 83;
    }
    steeringServo.write(theta);//set turning angle to theta
    vR = spd;
    vL = spd;
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
  prevAngle = theta;
}
  

//function stop()
//no parameters
//returns nothing
void stop(){
  if(prevAngle > 90){
    steeringServo.write(83);
  }else{
    steeringServo.write(90);//set wheels straight
  }
  currSpd = 0;
  //stop power to wheels
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);
  //set drive speed to 0
  analogWrite(pinENA12, 0);
  analogWrite(pinENA34, 0);
}

void maneuver(int choice){
  if(choice == 1){
    //figure 8
    stop();
    drive(135, true, 96);
    delay(3000);
    drive(45, true, 96);
    delay(3000);
    
  } else if(choice == 2){
    // Back up to turn right
    stop();
    delay(1000);
    drive(90, true, 128);
    delay(2000);
    drive(110, false, 128);
    delay(2000);
    drive(90, true, 128);
    delay(2000);
    drive(70, false, 128);
    delay(2000);
    stop();
    drive(90, true, 128);
    delay(2000);
    stop();
    
  } else if(choice == 3){
    // Back up to turn left
    stop();
    delay(1000);
    drive(90, true, 128);
    delay(2000);
    drive(70, false, 128);
    delay(2000);
    drive(90, true, 128);
    delay(2000);
    drive(110, false, 128);
    delay(2000);
    stop();
    drive(90, true, 128);
    delay(2000);
    stop();
  } else {
    stop();
  }
}

void loop()
{
  if(Serial.available()){
   
    cmdRec = Serial.readStringUntil('\n');
    cmd = cmdRec.substring(0,3);
    value = cmdRec.substring(3,6);
    intValue = value.toInt();
//    
//    //Serial.print("recieved");
//    Serial.println(cmd);
//    //Serial.print("and ");
//    Serial.println(value);
  }
  
  if(cmd == "FWD"){
    
    drive(90, true, intValue);
    
  } else if(cmd == "BWD"){
    
    drive(90, false, intValue);
    
  } else if(cmd == "STR"){
    
    drive(intValue, currDir, currSpd);
    
  } else if(cmd == "STP"){
    
    stop();
    
  } else if(cmd == "MAN"){
    
    maneuver(intValue);
    
  }
}