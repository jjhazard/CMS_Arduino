#include <CMS_Definitions.h>
#include <Codes.h>
#include <Transceiver.h>

Transceiver Node(2,2);

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Started Serial");
  if (!Node.start()) while (true);
  Serial.println("Started Lora");
  Node.send("1234", DISPATCH);
  Serial.println("Dispatched 1234");
}

void loop() {
  Node.check();
}
