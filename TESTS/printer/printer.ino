#define Printer Serial1

typedef enum {
       H_T =  9,
       L_F = 10,
       C_R = 13,
    ESCAPE = 27,
       G_S = 29,
     EX_PT = 33,
     QUOTE = 34,
      DASH = 45,
      ZERO = 48,
       ONE = 49,
       TWO = 50,
        AT = 64,
   J_UPPER = 70,
   A_LOWER = 97,
   D_LOWER = 100
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
void leftJustify() {
  Printer.write(ESCAPE);
  Printer.write(A_LOWER);
  Printer.write((uint8_t)0);
}
void centerJustify() {
  Printer.write(ESCAPE);
  Printer.write(A_LOWER);
  Printer.write((uint8_t)1);
}
void rightJustify() {
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
  Printer.write(G_S);
  Printer.write(EX_PT);
  Printer.write(multiplier);
}
void widthRatio(uint8_t multiplier) {
  if (!multiplier || multiplier > 8) {
    return;
  }
  multiplier--;
  Printer.write(G_S);
  Printer.write(EX_PT);
  Printer.write((multiplier << 4));
}
void sizeRatio(uint8_t multiplier) {
  if (!multiplier || multiplier > 8) {
    return;
  }
  multiplier--;
  Printer.write(G_S);
  Printer.write(EX_PT);
  Printer.write(((multiplier << 4) | multiplier));
}
//////////////////////

const char input[] = "1234";

void setup() {
  Printer.begin(9600, SERIAL_8N1);
  while (!Printer);
}

void loop() {
  feedLines(1);
  sizeRatio(1);
  leftJustify();
  Printer.print("Your Free-Air Code is:");
  feedLines(2);
  centerJustify();
  sizeRatio(3);
  Printer.write(input);
  feedLines(4);
  sizeRatio(1);
  leftJustify();
  Printer.print("Your Code is good for six hours");
  feedLines(1);
  Printer.print("and expires upon use. Enjoy!");
  feedLines(6);
  while (true);
}
