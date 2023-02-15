#include <Transceiver.h>
#include <Peripherals.h>

void setup() {
  setUpOutput();
}

void loop() {
  
  #define THIS_NODE   2
  #define TOTAL_NODES 2
  Transceiver Node(THIS_NODE, TOTAL_NODES);
  #undef  THIS_NODE
  #undef  TOTAL_NODES
  beep();
  delay(1000);
  String codeString;

  if (!Node.start()) {
    while(true);
  }
  beep();
  delay(1000);

  Node.check();
  beep();
  delay(1000);
  if (codeString != "") {
    Node.send(codeString, DISPATCH);
  }
  beep();
  delay(1000);
  while (true);
}
