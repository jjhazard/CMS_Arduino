#include <Peripherals.h>
#include <Arduino.h>

const int button_pin = 12;
const int buzzer_pin = 12;
const int  relay_pin = 13;

void setUpInput() {
  pinMode(button_pin, INPUT_PULLUP);
  randomSeed(analogRead(0));
}

void setUpOutput() {
  pinMode(buzzer_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
}

void beep() {
  for (int i = 0; i < 285; i++) {   // for the calculated length of time...
    digitalWrite(buzzer_pin, HIGH);  // write the buzzer pin high to push out the diaphram
    delayMicroseconds(169);         // wait for the calculated delay value
    digitalWrite(buzzer_pin, LOW);   // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(169);         // wait again or the calculated delay value
  }
}

void relayOn() {
  digitalWrite(relay_pin, HIGH);
}

void relayOff() {
  digitalWrite(relay_pin, LOW);
}

bool pressed()  {
  return !digitalRead(button_pin);
}