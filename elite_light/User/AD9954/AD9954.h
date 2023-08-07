#ifndef __AD9954_H
#define __AD9954_H    
 
#include "main.h"    //IO口控制
 
/*  引脚设置
**      AD9954_CS----------PF12         OUT
**      AD9954_SCLK--------PF14         OUT
**      AD9954_SDIO--------PG0         OUT
**      AD9954_OSK---------PE7       OUT
**      PS0----------------PE9        OUT
**        PS1----------------PE11         OUT
**      IOUPDATE-----------PE13          OUT
**        AD9954_SDO---------PF11          IN
**      AD9954_IOSY--------PF13          OUT
**      AD9954_RES---------PF15        OUT
**      AD9954_PWR---------PG1        OUT
*/
 
#define AD9954_CS     PFout(12)            
#define AD9954_SCLK PFout(14)            
#define AD9954_SDIO PGout(0)            
#define AD9954_OSK     PEout(7)            
#define PS0         PEout(9)        
#define PS1         PEout(11)            
#define IOUPDATE     PEout(13)            
#define AD9954_SDO    PFin(11)        
#define AD9954_IOSY PFout(13)        
#define AD9954_RES     PFout(15)        
#define AD9954_PWR     PGout(1)    
 
 
#define CFR1    0X00            
#define CFR2    0X01
#define ASF     0X02
#define ARR1    0X03
#define FTW0    0X04
#define POW0    0X05
#define FTW1    0X06
#define NLSCW   0X07
#define PLSCW   0X08
#define RSCW0   0X07
#define RSCW1   0X08
#define RSCW2   0X09
#define RSCW3   0X0A
#define RAM     0X0B
 
#define No_Dwell 0X80
 
typedef enum {
DownScan  =   0,
UpScan,
DoubleScan
}ScanMode;              
 
void GPIO_AD9954_Init(void);
void AD9954_RESET(void);
void AD9954_SENDBYTE(u8 dat);
u8 AD9954_ReadByte(void);
void UPDATE(void);

void AD9954_Init(void);                 //DDS初始化
void AD9954_SETFRE(double f);               //设置频率
void Write_ASF(u16 data);                  //设置幅值
 
#endif