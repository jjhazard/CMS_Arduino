#include <Peripherals.h>

void setup() {
  setUpOutput();
  Serial.begin(9600);
  while(!Serial);
}

unsigned long Time = 0;
void loop() {
  Time = millis();
  beep();
  Serial.println(String(millis() - Time));
  delay(5000);
}
