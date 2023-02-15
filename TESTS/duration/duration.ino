#include <Codes.h>
#include <Peripherals.h>
#include <Transceiver.h>

#define NODE_ID     2
#define TOTAL_NODES 2
Transceiver Node(NODE_ID, TOTAL_NODES);
#undef  NODE_ID
#undef  TOTAL_NODES

void setup() {
  setUpOutput();
  Serial.begin(9600);
  while(!Serial) {beep(); delay(100);};
  if (!Node.start()) while(true) {beep(); delay(1000);};
}
 
String sendThis;
void loop() {
  monitorTimers();
  Node.check();
  if (Serial.available()) {
    while (Serial.available())  {
      sendThis += (char)Serial.read();
    }
    sendThis = sendThis.substring(0, 4);
    uint32_t thisCode = sendThis.toInt();
    if (dispatch(thisCode)) {
      Serial.print(sendThis);
      Serial.print(" dispatched at ");
      Serial.println(millis());
      Node.send(sendThis, DISPATCH);
    } else if (expire(thisCode)) {
      Serial.print(sendThis);
      Serial.print(" expired at ");
      Serial.println(millis());
      Node.send(sendThis, EXPIRE);
    }
    sendThis = "";
  }
}
