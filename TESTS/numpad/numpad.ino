#include <Numberpad.h>
#include <Codes.h>

void setup() {
  beginNumberpad();
  dispatch(1234);
  dispatch(1235);
  dispatch(1236);
  dispatch(1237);
}
String got;
void loop() {
  got = checkNumberpad();
}
