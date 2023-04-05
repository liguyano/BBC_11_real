//
// Created by kurum on 西暦2023年4月5日.
//

#include "PCF8591.h"
void pcf_write(unsigned char value)
{
    one_start();
    one_sendByte(ADD);
    I2C_recv_ack();
    one_sendByte(0x41);
    I2C_recv_ack();
    one_sendByte(value);
    I2C_recv_ack();
    one_stop();
}
unsigned char pcf_read(uchar command)
{
    uchar result;
    one_start();
    one_sendByte(ADD);
    I2C_recv_ack();
    one_sendByte(command);
    I2C_recv_ack();
    one_start();
    one_sendByte(0x91);
    I2C_recv_ack();
    result=I2C_ReceiveByte();
    I2C_send_ac(1);
    one_stop();
    return result;
}