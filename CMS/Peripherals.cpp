#include <Peripherals.h>
#include <Arduino.h>

#define timedOn(pin, duration) {digitalWrite((pin), HIGH);\
                                delay((duration));\
                                digitalWrite((pin), LOW);}

const int buzzerPin = 6;
const int  relayPin = 7;
const int buttonPin = 6;

void setUpOutput() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
}

void setUpInput() {
  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(analogRead(0));
}

void beep()     timedOn(buzzerPin, TENTH_SECOND);
void activate() timedOn(relayPin, THREE_SECONDS);
bool pressed()  {return !digitalRead(buttonPin);}
