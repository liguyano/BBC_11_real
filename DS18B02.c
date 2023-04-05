/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10
  硬件环境: CT107单片机综合实训平台(外部晶振12MHz) STC89C52RC单片机
  日    期: 2011-8-9
*/
#include "STC15F2K60S2.h"
#include "DS18B02.h"

sbit DQ = P1^4;  //单总线接口

//单总线延时函数
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
    unsigned char i;
    while(t--){
        for(i =0;i<12;i++)
            ;
    }
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        DQ = 0;
        DQ = dat&0x01;
        Delay_OneWire(5);
        DQ = 1;
        dat >>= 1;
    }
    Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
    unsigned char i;
    unsigned char dat;

    for(i=0;i<8;i++)
    {
        DQ = 0;
        dat >>= 1;
        DQ = 1;
        if(DQ)
        {
            dat |= 0x80;
        }
        Delay_OneWire(5);
    }
    return dat;
}

//DS18B20设备初始化
bit init_ds18b20(void)
{
    bit initflag = 0;

    DQ = 1;
    Delay_OneWire(12);
    DQ = 0;
    Delay_OneWire(80);
    DQ = 1;
    Delay_OneWire(10);
    initflag = DQ;
    Delay_OneWire(5);

    return initflag;
}

unsigned int rd_temperature(void){
    unsigned char low,high;
    unsigned int result;

    init_ds18b20();
    Write_DS18B20(0xcc);
    Write_DS18B20(0x44);
    Delay_OneWire(200);

    init_ds18b20();
    Write_DS18B20(0xcc);
    Write_DS18B20(0xbe);
    low = Read_DS18B20();
    high = Read_DS18B20();
    result = high&0x0f;
    result <<= 8;
    result |= low;
    result = result*6.25;//0.0625*100;

    return result;
}