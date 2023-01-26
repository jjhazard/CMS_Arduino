#include <Requests.h>
#include <KeyboardController.h>
#include <Peripherals.h>
#include <Codes.h>

#define Printer Serial1

typedef enum {
      H_TAB =  9,
     L_FEED = 10,
   C_RETURN = 13,
     ESCAPE = 27,
  GROUP_SEP = 29,
  EXC_POINT = 33,
      QUOTE = 34,
       DASH = 45,
       ZERO = 48,
        ONE = 49,
        TWO = 50,
         AT = 64,
    J_UPPER = 70,
    A_LOWER = 97,
    D_LOWER = 100,
  L_BRACKET = 123
} COMMANDS;

//////////////////////
/*
  Initialize
*/
void initialize() {
  Printer.write(ESCAPE);
  Printer.write(AT);
}

//////////////////////
/*
  Print and feed lines
*/
void feedLines(uint8_t lines) {
  Printer.write(ESCAPE);
  Printer.write(D_LOWER);
  Printer.write(lines);
}

//////////////////////
/*
  Alignment
*/
void leftAlign() {
  Printer.write(ESCAPE);
  Printer.write(A_LOWER);
  Printer.write((uint8_t)0);
}
void centerAlign() {
  Printer.write(ESCAPE);
  Printer.write(A_LOWER);
  Printer.write((uint8_t)1);
}
void rightAlign() {
  Printer.write(ESCAPE);
  Printer.write(A_LOWER);
  Printer.write((uint8_t)2);
}

//////////////////////
/*
  Underline
*/
void underlineOff() {
  Printer.write(ESCAPE);
  Printer.write(DASH);
  Printer.write(ZERO);
}
void underlineThin() {
  Printer.write(ESCAPE);
  Printer.write(DASH);
  Printer.write(ONE);
}
void underlineThick() {
  Printer.write(ESCAPE);
  Printer.write(DASH);
  Printer.write(TWO);
}

//////////////////////
/*
  Size alteration
*/
void heightRatio(uint8_t multiplier) {
  if (!multiplier || multiplier > 8) {
    return;
  }
  multiplier--;
  Printer.write(GROUP_SEP);
  Printer.write(EXC_POINT);
  Printer.write(multiplier);
}
void widthRatio(uint8_t multiplier) {
  if (!multiplier || multiplier > 8) {
    return;
  }
  multiplier--;
  Printer.write(GROUP_SEP);
  Printer.write(EXC_POINT);
  Printer.write((multiplier << 4));
}
void sizeRatio(uint8_t multiplier) {
  if (!multiplier || multiplier > 8) {
    return;
  }
  multiplier--;
  Printer.write(GROUP_SEP);
  Printer.write(EXC_POINT);
  Printer.write(((multiplier << 4) | multiplier));
}

//////////////////////
/*
  Rotate and flip
*/
void upsideDownOff() {
  Printer.write(ESCAPE);
  Printer.write(L_BRACKET);
  Printer.write((uint8_t)0);
}
void upsideDownOn() {
  Printer.write(ESCAPE);
  Printer.write(L_BRACKET);
  Printer.write((uint8_t)1);
}

//////////////////////

void beginRequests() {
  setUpInput();
  Printer.begin(9600, SERIAL_8N1);
  while (!Printer);
  initialize();
  feedLines(1);
  upsideDownOn();
}

void printCode(String digits) {

  //Everything prints upside down, so we have to print the lines in reverse
  Printer.print("and expires upon use. Enjoy!");
  feedLines(1);

  Printer.print("Your Code is good for six hours");
  feedLines(2);

  centerAlign();
  sizeRatio(3);
  Printer.print(digits);
  feedLines(4);

  sizeRatio(1);
  leftAlign();
  Printer.print("Your Free-Air Code is:");
  feedLines(8);
}

unsigned long lastRequest = 0;

//Perform predetermined behavior based on which key is pressed
String checkRequests() {
  if (pressed()) {
    if (!lastRequest) {
      lastRequest = millis();
      uint16_t code = (uint16_t)random(0, 10000);
      while (!dispatch(code)) {
        if (code < 10000) {
          code++;
        } else {
          code = 0;
        }
      }
      String digits = String(code);
      while (digits.length() < 4) {
        digits = "0" + digits;
      }
      printCode(digits);
      return digits;
    } else if (timePassedSince(lastRequest, ONE_SECOND)) {
      lastRequest = 0;
    }
  }
  return "";
}