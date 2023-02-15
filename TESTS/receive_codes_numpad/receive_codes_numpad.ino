#include <CMS_Definitions.h>
#include <Numberpad.h>
#include <Transceiver.h>

void setup() {
  beginInput();
  if (!beginLoRa()) while (1);
}
String received_code;
void loop() {
  if (timePassedSince(lastIncrement, TEN_MINUTES)) incrementTimers();
  if (signal()) {
    received_code = receive();
    if (received_code != "") {
      dispatch(received_code.toInt());
      received_code = "";
    }
  }
}
void loop() {
  checkNumberpad();
  if (timePassedSince(lastIncrement, TEN_MINUTES)) incrementTimers();
  if (LoRa.parsePacket()) {
    String received;
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }
    dispatch(received.toInt());
  }
}
