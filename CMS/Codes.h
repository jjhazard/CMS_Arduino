#ifndef _CODES_H_
#define _CODES_H_

#include <CMS_Definitions.h>

bool available(uint32_t code);
bool dispatched(uint32_t code);
bool dispatch(uint32_t code);
bool expired(uint32_t code);
bool expire(uint32_t code);
bool process(uint32_t code);
void monitorTimers();

#endif