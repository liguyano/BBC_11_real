//
// Created by kurum on 西暦2023年4月5日.
//

#ifndef BBC_11_REAL_PCF8591_H
#define BBC_11_REAL_PCF8591_H
#include "SSS.h"
#include "I2C.h"
#define ADD 0x90
void pcf_write(unsigned char value);
unsigned char pcf_read(uchar command);
#endif //BBC_11_REAL_PCF8591_H
