#include <Numberpad.h>
#include <KeyboardController.h>
#include <Peripherals.h>
#include <Codes.h>

USBHost usb;
KeyboardController Numberpad(usb);

String input = "";
bool finished = false;
unsigned long lastButtonPress = 0;

#define ENTER 35
#define CLEAR 42

//clear the test variables for input gathering
void clear() {
  input = "";
  finished = false;
  lastButtonPress = 0;
}

//beep as many times as requested, then clear
void reject(uint8_t count=3) {
  for (uint8_t sound=0; sound<count; sound++) {
    beep();
    delay(FIFTEENTH_SECOND);
  }
  clear();
}

//Setup output peripheral pins
void beginNumberpad() {
  setUpOutput();
}

//Check if input is being timed, check for input, then check if the input process is complete
String checkNumberpad() {
  usb.Task();
  if (lastButtonPress) {
    if (finished) {
      String valid_code = input;
      clear();
      return valid_code;
    }
    if (timePassedSince(lastButtonPress, FIVE_SECONDS)) reject();
  }
  return "";
}

//Perform predetermined behavior based on which key is pressed
void keyPressed() {
  beep();
  lastButtonPress = millis();
  uint8_t key = Numberpad.getKey();
  switch(key) {
    case ENTER:
      if (input.length() == 4 && dispatch(input.toInt())) finished = true;
      else reject();
      break;
    case CLEAR:
      reject(2);
      break;
    default:
      if (input.length() == 63) reject();
      else input += (char)key;
  }
}