#include <Transceiver_Test.h>
#include <Codes.h>

#define NODE_ID     2
#define TOTAL_NODES 2
TTransceiver Node(NODE_ID, TOTAL_NODES);
#undef  NODE_ID
#undef  TOTAL_NODES

String codeString;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  if(!Node.start()) while (true);
}

void getInput(){
  if (Serial.available()) {
    while (Serial.available()) codeString += (char)Serial.read();
    codeString = codeString.substring(0,4);
  }
}

void loop() {
  Node.check();
  getInput();
  if (codeString != "") {
    uint32_t code = codeString.toInt();
    if (dispatch(code)) {
      Serial.println("Dispatching..." + codeString);
      Node.send(codeString, DISPATCH);
    } else if (expire(code)) {
      Serial.println("Expiring..." + codeString);
      Node.send(codeString, EXPIRE);
    }
    codeString = "";
  }
}
