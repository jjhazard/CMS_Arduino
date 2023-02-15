#include <CMS_Definitions.h>
#include <Transceiver.h>
#include <Codes.h>
#include <Peripherals.h>

void setup() {
  Serial.begin(9600);
  while(!Serial);
  setUpOutputPins();
  if (!beginLoRa()) while (1);
}

String hold;
void loop() {
  checkTransceiver();
  if (Serial.available()) {
    while (Serial.available()) {
      hold += (char)Serial.read();
    }
    hold.trim();
    Serial.print(hold);
    if (expire(hold.toInt())) Serial.println(" Expired.");
    else Serial.println(" Rejected."); 
    hold = "";
  }
}
