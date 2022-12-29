#include <Codes.h>

uint8_t data[10000] = {0};
unsigned long lastIncrement = 0;

#define EXISTS(X)      (code<10000)
#define NOT_EXISTS(X)  (code>9999)
#define AVAILABLE(X)   (!data[(X)])
#define TIMING(X)      (data[(X)])
#define INCREMENT(X)   (data[(X)]+=0x01)
#define DISPATCHED(X)  (data[(X)] & data[(X)]<0X024)
#define DISPATCH(X)    (data[(X)]=0X01)
#define EXPIRED(X)     (data[(X)]>0x24)
#define EXPIRE(X)      (data[(X)]=0x25)

bool available(uint32_t code) {
  if (EXISTS(code) && AVAILABLE(code)) return true;
  return false;
}
bool dispatched(uint32_t code) {
  if (EXISTS(code) && DISPATCHED(code)) return true;
  return false;
}
bool dispatch(uint32_t code) {
  if (EXISTS(code) && AVAILABLE(code)) DISPATCH(code);
  else return false;
  return true;
}
bool expired(uint32_t code) {
  if (EXISTS(code) && EXPIRED(code)) return true;
  return false;
}
bool expire(uint32_t code) {
  if (EXISTS(code) && DISPATCHED(code)) EXPIRE(code);
  else return false;
  return true;
}
bool process(uint32_t code) {
  if      (NOT_EXISTS(code)) return false; 
  if      (AVAILABLE(code))  DISPATCH(code);
  else if (DISPATCHED(code)) EXPIRE(code);
  else    return false;
  return true;
}
void monitorTimers() {
  if (timePassedSince(lastIncrement, TEN_MINUTES))
    for (uint16_t code = 0; code < 10000; code++)
      if (TIMING(code)) INCREMENT(code);
}