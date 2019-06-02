#include "Adafruit_VL53L0X.h"
#include <Servo.h>


const double turnParam = 1;
int rightMotor = 2;
int leftMotor = 3;
int trigPin = 5;
int echoPin = 6;
int servoPin = 7;
int servoPower = 8;
Servo myServo;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  pinMode(rightMotor, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  pinMode(servoPower, OUTPUT);
  myServo.attach(servoPin, 600, 2300);
  
  digitalWrite(servoPower, HIGH);
  myServo.write(1450);
  delay(200);
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}
void loop() {
  //tahan liikekoodi
  int dist = Range();
  
  delay(50);
  Serial.print("eka ping ");
  Serial.println(dist);
  int suunta;
  if(dist < 300){
    StopMotors();
    suunta = scan();
    
    Serial.print(suunta);
    if(suunta < 0)
      Right(suunta);
    else
      Left(suunta);
  }
  MotorsOn();
}

int Range() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  int dist;
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    dist = (int)measure.RangeMilliMeter;
    Serial.print("Distance (mm): "); Serial.println(dist);
  } else {
    Serial.println(" out of range ");
  }
  
  delay(100);
  return dist;
}
int scan() {
  delay(300);
  for(int i = 0; i <= 17; ++i){
    digitalWrite(servoPower, HIGH);
    myServo.write((600 + 100*i));
    delay(500);
    int dist2 = Range();
    delay(300);
    Serial.print("Scan ping ");
    Serial.println(dist2);
    if(dist2 > 200){
      myServo.write(1450);
      delay(500);
      return (100*i-850);
    }
  }
  myServo.write(1450);
  delay(500);
  return 0;
}
void Right(int suunta){
  analogWrite(rightMotor, 0);
  analogWrite(leftMotor, 255);
  Serial.println("right");
  delay(abs(turnParam * suunta));
  return;
}
void Left(int suunta){
  analogWrite(rightMotor, 0);
  analogWrite(leftMotor, 255);
  Serial.println("left");
  delay(abs(turnParam * suunta));
  return;
}
void MotorsOn() {
  analogWrite(rightMotor, 255);
  analogWrite(leftMotor, 255);
}
void StopMotors() {
  analogWrite(rightMotor, 0);
  analogWrite(leftMotor, 0);
}
