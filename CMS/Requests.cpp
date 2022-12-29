#include <Requests.h>
#include <KeyboardController.h>
#include <Peripherals.h>
#include <Codes.h>

//1b401d10111b6100
const char smallLeft[] = {0x1b, 0x40, 0x1d, 0x10, 0x11, 0x1b, 0x61, 0x00};
const char  nextLine[] = {0x0d, 0x0a};
const char bigCenter[] = {0x1b, 0x40, 0x1d, 0x21, 0x11, 0x1b, 0x61, 0x01};
const char feedLines[] = {0x0a, 0x0a, 0x0a, 0x0a};
unsigned long lastRequest = 0;

//Setup output peripheral pins
void beginRequests() {
  setUpInput();
  Serial.begin(9600);
  while (!Serial);
}

//Perform predetermined behavior based on which key is pressed
String checkRequests() {
  if (pressed()) {
    if (!lastRequest) {
      uint16_t code = (uint16_t)random(0, 10000);
      while (!dispatch(code)) {
        if (code < 10000) code++;
        else code = 0;
      }
      String digits = String(code);
      lastRequest = millis();
      return digits;
    } else if (timePassedSince(lastRequest, HALF_SECOND)) {
      lastRequest = 0;
    }
  }
  return "";
}