#include <LedHelper.h>
#include <PhotoresistorHelper.h>
#include <PotentiometerHelper.h>
#include <Servo.h>

const byte photoresistorPin = 0, potentiometerPin = 1, ledPin = 9, servoPin = 8;
Servo servo1;
LED led = LED(ledPin);
Photoresistor photoresistor = Photoresistor(photoresistorPin);
Potentiometer potentiometer = Potentiometer(potentiometerPin);
