#include <Codes.h>

uint8_t data[10000] = {0};
unsigned long last_increment = 0;


//Comparisons
#define EXISTS(X)      (code <  10000)
#define NOT_EXISTS(X)  (code >= 10000)
#define AVAILABLE(X)   (data[(X)] == 0x0000)
#define TIMING(X)      (data[(X)] != 0x0000)
#define DISPATCHED(X)  (TIMING((X)) && (data[(X)] <  0X0025))
#define EXPIRED(X)     (TIMING((X)) && (data[(X)] >= 0X0025))

//Assignments
#define DISPATCH(X)    (data[(X)] =  0X0001)
#define EXPIRE(X)      (data[(X)] =  0x0025)
#define INCREMENT(X)   (data[(X)] += 0x0001)

bool available(uint32_t code) {
  if (EXISTS(code) && AVAILABLE(code)) {
    return true;
  }
  return false;
}
bool dispatched(uint32_t code) {
  if (EXISTS(code) && DISPATCHED(code)) {
    return true;
  }
  return false;
}
bool dispatch(uint32_t code) {
  if (EXISTS(code) && AVAILABLE(code)) {
    DISPATCH(code);
  } else {
    return false;
  }
  return true;
}
bool expired(uint32_t code) {
  if (EXISTS(code) && EXPIRED(code)) {
    return true;
  }
  return false;
}
bool expire(uint32_t code) {
  if (EXISTS(code) && DISPATCHED(code)) {
    EXPIRE(code);
  } else {
    return false;
  }
  return true;
}
bool process(uint32_t code) {
  if (NOT_EXISTS(code)) {
    return false; 
  }
  if (AVAILABLE(code)) {
    DISPATCH(code);
  } else if (DISPATCHED(code)) {
    EXPIRE(code);
  } else {
    return false;
  }
  return true;
}
void monitorTimers() {
  if (timePassedSince(last_increment, TEN_MINUTES)) {
    last_increment = millis();
    for (uint16_t code = 0; code < 10000; code++) {
      if (TIMING(code)) {
        INCREMENT(code);
      }
      
    }
  }
}