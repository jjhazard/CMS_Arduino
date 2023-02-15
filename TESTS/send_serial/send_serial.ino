#include <Codes.h>
#include <Peripherals.h>
#include <Transceiver.h>

#define NODE_ID     2
#define TOTAL_NODES 2
Transceiver Node(NODE_ID, TOTAL_NODES);

void setup() {
  setUpOutput();
  Serial.begin(9600);
  while(!Serial) {beep(); delay(100);};
  if (!Node.start()) while(true) {beep(); delay(1000);};
}

#undef  NODE_ID
#undef  TOTAL_NODES

String sendThis;
void loop() {
  monitorTimers();
  Node.check();
  if (Serial.available()) {

    while (Serial.available()) {
      sendThis += (char)Serial.read();
    }
    sendThis = sendThis.substring(0, 4);
    uint32_t thisCode = sendThis.toInt();
    Serial.println(thisCode);
    if (available(thisCode)) {
      if (dispatched(thisCode) || expired(thisCode)) {
        Serial.println("Contradicted available.");
        sendThis = "";
        return;
      }
      Serial.println("Is available. Attempting Dispatch.");
      if (dispatch(thisCode)) {
        if (!available(thisCode) && dispatched(thisCode) && !expired(thisCode)) {
          Serial.println("Successfully verified dispatched.");
          Node.send(sendThis, DISPATCH);
          sendThis = "";
          return;
        } else
          Serial.println("Failed to verify dispatched");
      } else Serial.println("Failed to dispatch.");
    } else if (dispatched(thisCode)) {
      if (available(thisCode) || expired(thisCode)) {
        Serial.println("Contradicted dispatched.");
        sendThis = "";
        return;
      }
      Serial.println("Is dispached. Attempting Expire.");
      if (expire(thisCode)) {
        if (!available(thisCode) && !dispatched(thisCode) && expired(thisCode)) {
          Serial.println("Successfully verified expired.");
          Node.send(sendThis, EXPIRE);
          sendThis = "";
          return;
        } else
          Serial.println("Failed to verify expire.");
      } else Serial.println("Failed to expire.");
    } else if (expired(thisCode)) {
      if (available(thisCode) || dispatched(thisCode)) {
        Serial.println("Contradicted expired.");
        sendThis = "";
        return;
      }
      Serial.println("Is Expired.");
      if (!available(thisCode) && !dispatched(thisCode) && expired(thisCode))
        Serial.println("Successfully verified expired.");
      else
        Serial.println("Failed to verify expire.");
    } else Serial.println("Invalid Code.");
    sendThis = "";
  }
}
