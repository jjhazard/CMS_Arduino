#include <Peripherals.h>

void setup() {
  setUpInput();
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  if (pressed()) {
    Serial.println("Pressed");
    delay(500);
  }
}
