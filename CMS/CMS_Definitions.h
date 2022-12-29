#ifndef _CMS_DEFINITIONS_H_
#define _CMS_DEFINITIONS_H_

#include <Arduino.h>
#include <stdint.h>

typedef enum {
  FAILED_PARSE,
  DISPATCH,
  DISPATCH_ACK,
  EXPIRE,
  EXPIRE_ACK
} PACKET_TYPE;

typedef enum {
  FIFTEENTH_SECOND = 67,
  TENTH_SECOND     = 100,
  HALF_SECOND      = 500,
  THREE_SECONDS    = 3000,
  FIVE_SECONDS     = 5000,
  TEN_MINUTES      = 600000
} DURATION;

#define timePassedSince(UL, DURATION) ((millis()-(UL))>(DURATION))

#endif