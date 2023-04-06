#include <STC15F2K60S2.H>
#include <MATH.H>
#include "SSS.h"
#include "MAtrix_key.h"
#include "DS1302.h"
#include "DS18B02.h"
#include "PCF8591.h"
static unsigned char mode=0;
static bit D=1;
static bit Light =0;
static bit pre_Light =1;
static uchar lightTime=0;
// time temporotry light
static uint temp=567;
static uchar voltage;
static uchar para[]={17,25,4};
static uchar re_para[]={0x17,25,4};
unsigned char time[3]={24,0,0};
unsigned char code SMG_duanma[18]=
        {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
         0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
         0xbf,0x7f};/*用于数码管段选*/
uchar code otherDuan[]={0xc6,0xbf,0x8c,0x86}; //C - P E
uchar t_disPlay[8]={0,0,0xff,0xff,0xff,0xff,0xff,0xff};
static uchar light;
void SelectHC573(unsigned char channel)/*74HC573锁存器*/
{
    switch(channel)
    {
        /*
         * &0x1f 将前三位置0
         * | 0x80 将第一位置1
         * | 0xa0 1010 0000
         * */

        case 4:
            P2 = (P2 & 0x1f) | 0x80;
            break;
        case 5:
            P2 = (P2 & 0x1f) | 0xa0;
            break;
        case 6:
            P2 = (P2 & 0x1f)| 0xc0;//1100
            break;
        case 7:
            P2 = (P2 & 0x1f) | 0xe0;
            break;
    }
}void DIAPlaySMG_Bit(unsigned char value, unsigned char pos)/*数码管显示*/
{
    P2&=0x1f;
    P0 = 0x00;
    /*段选*/
    SelectHC573(6);
    P0 = 0x01 << pos;
    P0=P0;
    P2&=0x1f;
    /*位选*/
    SelectHC573(7);
    P0 = value;
    P2&=0x1f;
}
void read_time()
{
    time[2]=Read_Ds1302_Byte(0x81);//secend
    time[2]<<=1;
    time[2]>>=1;
    time[1]=Read_Ds1302_Byte(0x83);
    time[1]<<=1;
    time[1]>>=1;
}
void Timer0_Init(void)		//100us@11.0592MHz
{
    AUXR &= 0x7F;			//Timer clock is 12T mode
    TMOD &= 0xF0;			//Set timer work mode
    TMOD |= 0x01;			//Set timer work mode
    TL0 = 0x66;				//Initial timer value
    TH0 = 0xFC;				//Initial timer value
    TF0 = 0;				//Clear TF0 flag
    TR0 = 1;				//Timer0 start run
    EA=1;
    ET0=1;
}
void disPlay()
{uchar i;
    void showIntNUm(unsigned int num);
    switch (mode) {//time
        case 0:
            if(D)
            {
                read_time();
                t_disPlay[6]=SMG_duanma[time[2]>>4];
                t_disPlay[7]=SMG_duanma[time[2]&0x0f];
                t_disPlay[5]=otherDuan[1];
                //secend;
                t_disPlay[0]=SMG_duanma[time[0]>>4];
                t_disPlay[1]=SMG_duanma[time[0]&0x0f];
                t_disPlay[2]=otherDuan[1];

                t_disPlay[3]=SMG_duanma[time[1]>>4];
                t_disPlay[4]=SMG_duanma[time[1]&0x0f];
            } else
            {

                t_disPlay[0]=otherDuan[2];
                t_disPlay[1]=SMG_duanma[1];
                for (i=2;i<=5;++i)
                {
                    t_disPlay[i]=0xff;
                }
                t_disPlay[6]=SMG_duanma[para[0]/10];
                t_disPlay[7]=SMG_duanma[para[0]%10];
            }
            break;
        case 1:
            //temp
            if (D)
            {
                //showIntNUm(temp);
                for (i=1;i<=4;++i)
                {
                    t_disPlay[i]=0xff;
                }
                t_disPlay[0]=otherDuan[0];
                t_disPlay[5]=SMG_duanma[temp/1000]&0x7f;
                t_disPlay[6]=SMG_duanma[(temp%1000)/100];
                t_disPlay[7]=SMG_duanma[(temp%100)/10];
            } else
            {
                t_disPlay[0]=otherDuan[2];
                t_disPlay[1]=SMG_duanma[2];
                for (i=2;i<=5;++i)
                {
                    t_disPlay[i]=0xff;
                }
                t_disPlay[6]=SMG_duanma[para[1]/10];
                t_disPlay[7]=SMG_duanma[para[1]%10];
            }
            break;
        case 2:
            if(D)
            {

                t_disPlay[0]=otherDuan[3];
                t_disPlay[1]=0xff;
                t_disPlay[5]=0xff;
                t_disPlay[6]=0xff;
                t_disPlay[2]=SMG_duanma[voltage/100];
                t_disPlay[3]=SMG_duanma[voltage%100/10];
                t_disPlay[4]=SMG_duanma[voltage%10];
                t_disPlay[7]=SMG_duanma[Light];
            } else
            {  t_disPlay[0]=otherDuan[2];
                t_disPlay[1]=SMG_duanma[3];
                for (i=2;i<=6;++i)
                {
                    t_disPlay[i]=0xff;
                }
                t_disPlay[7]=SMG_duanma[para[2]];

            }
            break;

    }
    for (i=0;i<8;i++)
    {
        DIAPlaySMG_Bit(t_disPlay[i],i);
        Delay(100);
    }

}
void main()
{
    unsigned char key;
    light=0xff;
    SelectHC573(4);
    P0=0xff;
    P2&=0x1f;
    Timer0_Init();
    init_ds1302(0x16,0x59,0x50);
    while (1)
    {
        key=get_key();
        if (key<17) {
            Delay(10);
            switch (key) {
                case 4:
                    D=~D;
                    if(D)
                    {
                        if (para[0]>=10)
                            re_para[0]=16;
                        else
                            re_para[0]=0;
                        re_para[0] += (para[0]%10);
                        re_para[1]=para[1];
                        re_para[2]=para[2];
                    }
                    break;
                case 5://mode change
                    mode++;
                    mode %= 3;
                    break;
                case 8:
                    //minus
                    if(!D)
                        para[mode]--;
                    break;
                case 9:
                    //plus
                    if(!D)
                        para[mode]++;
                    break;
            }
            while (key < 17)
            {
                key=get_key();
                disPlay();
            }
        }
        disPlay();
    }//

}
void time0() interrupt NUM1
{
    static uint t;
    t++;
    if (!(t%100) && lightTime!=0)
    {
        lightTime++;
    }
    if (t>400)
    { temp=rd_temperature();
        voltage= pcf_read(0x43);
        time[0]= Read_Ds1302_Byte(0x85);
        time[0]<<=3;
        time[0]>>=3;
        if (re_para[0]<8)
        {
            if (time[0]<8 && time[0]>=re_para[0])
                light&=0xfE;
            else
                light|=0x01;
        } else
        {
            if(time[0]>=re_para[0])
                light&=0xfe;
            else
                light|=0x01;
        }
        if (temp<re_para[1])
        {
            light &= 0xfd;
        } else
        {
            light |= 0x02;
        }
        light|=0xf0;
        pre_Light=Light;
        if (voltage==0)
        {Light=1;
            if (lightTime>=30 || lightTime==0)
            {open_light:
                // open light.
                light &= 0xfb;
                light&= _crol_(0xfe,re_para[2]-1);
                lightTime=0;
            }
        } else
        {Light=0;
            //close light
            if (lightTime>=30 || lightTime==0)
            {close_light:
                light |= 0x04;
                lightTime=0;
            }

        }
        if (pre_Light!=Light)
        {
            if (lightTime==0)
            {
                lightTime=1;
            } else
            {
                lightTime=0;
            }
            pre_Light=Light;
            if (Light)
                goto close_light;
            else
                goto open_light;
        }
        t=0;
        SelectHC573(4);
        P0=light;
        P2&=0x1f;

    }
    TL0 = 0x66;				//Initial timer value
    TH0 = 0xFC;				//Initial timer value

}