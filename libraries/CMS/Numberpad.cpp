#include <Numberpad.h>
#include <KeyboardController.h>
#include <Peripherals.h>
#include <Codes.h>

USBHost usb;
KeyboardController Numberpad(usb);

String input = "";
bool code_accepted = false;
unsigned long last_button_press = 0;
unsigned long activated = 0;

typedef enum {
  ENTER = 13,
  ZERO  = 48,
  ONE   = 49,
  TWO   = 50,
  THREE = 51,
  FOUR  = 52,
  FIVE  = 53,
  SIX   = 54,
  SEVEN = 55,
  EIGHT = 56,
  NINE  = 57,
  CLEAR = 127
} KEY;

//clear the test variables for input gathering
void clear() {
  input = "";
  code_accepted = false;
  last_button_press = 0;
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
  if (activated && timePassedSince(activated, THREE_SECONDS)) {
    relayOff();
    activated = 0;
    clear();
  } else if (last_button_press) {
    if (code_accepted) {
      relayOn();
      activated = millis();
      String valid_code = input;
      clear();
      return valid_code;
    }
    if (timePassedSince(last_button_press, FIVE_SECONDS)) {
      reject();
    }
  }
  return "";
}

//Perform predetermined behavior based on which key is pressed
void keyPressed() {
  beep();
  last_button_press = millis();
  KEY button_pressed = (KEY)Numberpad.getKey();
  switch(button_pressed) {
    case ENTER:
      if (input.length() == 4 && expire(input.toInt())) {
        code_accepted = true;
      } else {
        reject();
      }
      break;
    case CLEAR:
      reject(2);
      break;
    default:
      if (input.length() == 63) {
        reject();
      } else {
        input += (char)button_pressed;
      }
  }
}