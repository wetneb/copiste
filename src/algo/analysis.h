#ifndef INCLUDEDANALYSISH
#define INCLUDEDANALYSISH

#include <vlc/vlc.h>
#include <iostream>

uint16_t* naiveDFT(uint16_t* source, int power);

float ZCR(uint16_t* source, unsigned int size);

#endif
