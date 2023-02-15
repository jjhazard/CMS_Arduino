

void setup() {
  Serial.begin(9600);
  while(!Serial);

}

void loop() {
  
  uint16_t code = 0;
  String digits = String(code);
  while (digits.length() < 4) {
    digits = "0" + digits;
  }
  Serial.println(digits);
  
  code = 1;
  digits = String(code);
  while (digits.length() < 4) {
    digits = "0" + digits;
  }
  Serial.println(digits);
  code = 11;
  digits = String(code);
  while (digits.length() < 4) {
    digits = "0" + digits;
  }
  Serial.println(digits);
  code = 111;
  digits = String(code);
  while (digits.length() < 4) {
    digits = "0" + digits;
  }
  Serial.println(digits);
  code = 1111;
  digits = String(code);
  while (digits.length() < 4) {
    digits = "0" + digits;
  }
  Serial.println(digits);
  while(true);
}
