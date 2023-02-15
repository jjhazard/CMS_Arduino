#include <Codes.h>
#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while(!Serial);
  if (!LoRa.begin(915E6)) {
    while (true);
  }
}

void loop() {
  if (LoRa.parsePacket()) {
    while(LoRa.available()) {
      Serial.print( String(  (int)LoRa.read()  ) );
    }
  }
}


/*
#include <Codes.h>
#include <Transceiver.h>

#define NODE_ID     1
#define TOTAL_NODES 2
Transceiver Node(NODE_ID, TOTAL_NODES);
#undef  NODE_ID
#undef  TOTAL_NODES

String codeString;

void setup() {
  if (!Node.start()) while (true);
}

void loop() {
  Node.check();
}
*/