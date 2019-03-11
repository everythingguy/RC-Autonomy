#include <Adafruit_MotorShield.h>

#define leftSensorPins[] = { 9, 10 };
#define rightSensorPins[] = { 11, 12 };
#define backSensorPins[] = { 1, 2 };
#define gasPin = 2;
#define steerPin = 3;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *gasMotor = AFMS.getMotor(gasPin);
Adafruit_DCMotor *steerMotor = AFMS.getMotor(steerPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(leftSensorPins[0], OUTPUT);
  pinMode(rightSensorPins[0], OUTPUT);

  pinMode(leftSensorPins[1], INPUT);
  pinMode(rightSensorPins[1], INPUT);

  AFMS.begin();

  gasMotor->setSpeed(60);

  steerMotor->setSpeed(255);

  Serial.begin(9600);
}

int checkSensor(int pins[]) {
  long duration;
  int distance;
  
  digitalWrite(pins[0], LOW);
  delayMicroseconds(2);

  digitalWrite(pins[0], HIGH);
  delayMicroseconds(10);
  digitalWrite(pins[0], LOW);

  duration = pulseIn(pins[1], HIGH);

  distance = duration * 0.034 / 2;

  return distance;
}

int meanCheckSensor(int pins[]) {
  int Nums[] = {checkSensor(pins), checkSensor(pins), checkSensor(pins), checkSensor(pins)};
  int mean = Nums[0] + Nums[1] + Nums[2] + Nums[3];
  mean = mean / 4;
  return mean;
}

void loop() {
  // put your main code here, to run repeatedly:
  int leftDist = checkSensor(leftSensorPins);
  int rightDist = checkSensor(rightSensorPins);
  int backDist = checkSensor(backSensorPins);

  int closeness = 15;
  int forwardSpeed = 90;
  int backwardSpeed = 150;

  Serial.print("Left: ");
  Serial.println(leftDist);

  Serial.print("Right: ");
  Serial.println(rightDist);

  if (backDist > 15) {
  if (leftDist < closeness and rightDist < closeness) {
    gasMotor->setSpeed(backwardSpeed);
    gasMotor->run(BACKWARD);
    steerMotor->run(BACKWARD);
  }
  else if(leftDist < closeness) {
    gasMotor->setSpeed(forwardSpeed);
    gasMotor->run(FORWARD);
    steerMotor->run(BACKWARD);
  }
  else if(rightDist < closeness) {
    gasMotor->setSpeed(forwardSpeed);
    gasMotor->run(FORWARD);
    steerMotor->run(FORWARD);
  }
  else {
    gasMotor->setSpeed(forwardSpeed);
    gasMotor->run(FORWARD);
    steerMotor->run(RELEASE);
  }
  }
  else {
    gasMotor->setSpeed(forwardSpeed);
    gasMotor->run(FORWARD);
    steerMotor->run(RELEASE);
  }
}
