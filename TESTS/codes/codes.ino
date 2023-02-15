#include <Codes.h>

void setup() {
  Serial.begin(9600);
  while(!Serial);
  dispatch(1234);
  if (dispatched(1234)) {
    Serial.println("1234 dispatched");
  } 
}
bool change = false;
void loop() {
  monitorTimers();
  if (!change && expired(1234)) {
    Serial.println("1234 expired");
    change = true;
  } else if (available(1234)) {
    Serial.println("1234 available");
  }
}
