#ifndef _CMS_DEFINITIONS_H_
#define _CMS_DEFINITIONS_H_

#include <Arduino.h>
#include <stdint.h>

typedef enum {
  DISPATCH     = 0,
  EXPIRE       = 1,
  DISPATCH_ACK = 2,
  EXPIRE_ACK   = 3
} PACKET_TYPE;

typedef enum {
  FIFTEENTH_SECOND = 67,
  TENTH_SECOND     = 100,
  ONE_SECOND       = 1000,
  THREE_SECONDS    = 3000,
  FIVE_SECONDS     = 5000,
  TEN_MINUTES      = 600000
} DURATION;

#define timePassedSince(UL, DURATION) ((millis()-(UL))>(DURATION))

#endif