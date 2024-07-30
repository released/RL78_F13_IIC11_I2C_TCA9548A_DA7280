/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include "inc_main.h"

#include "misc_config.h"
#include "custom_func.h"
#include "simple_i2c_drive.h"
#include "TCA9548A_driver.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_32bit flag_TCA9548A_CTL;
#define FLAG_TCA9548A_REVERSE0                 		    	(flag_TCA9548A_CTL.bit0)
#define FLAG_TCA9548A_REVERSE1                       		(flag_TCA9548A_CTL.bit1)
#define FLAG_TCA9548A_REVERSE2                 				(flag_TCA9548A_CTL.bit2)
#define FLAG_TCA9548A_REVERSE3                      		(flag_TCA9548A_CTL.bit3)
#define FLAG_TCA9548A_REVERSE4                       	    (flag_TCA9548A_CTL.bit4)
#define FLAG_TCA9548A_REVERSE5                              (flag_TCA9548A_CTL.bit5)
#define FLAG_TCA9548A_REVERSE6                              (flag_TCA9548A_CTL.bit6)
#define FLAG_TCA9548A_REVERSE7                              (flag_TCA9548A_CTL.bit7)

/*_____ D E F I N I T I O N S ______________________________________________*/

uint32_t u32TCA9548ATimeOutCnt = 0;

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void TCA9548A_reset(unsigned char rst)	// P44
{
	// active-low
	unsigned short timeout = 0xFFFF;
	
	if (rst)
	{		
		TCA9548A_RESET = 1;
	}
	else
	{
		// delay 6 ns
		TCA9548A_RESET = 0;
		while(timeout--);
	}

}

/*
	A0 : LOW
	A1 : LOW
	A2 : LOW
	
	==> 1 1 1 0 A2 A1 A0 R/W
	==> 0xE0 / 0xE1 (8bit) , 
	==> 0x70 (7bit)
	
*/


MD_STATUS TCA9548A_SetChannel(unsigned char channel)
{
	unsigned char ch = 0;
	unsigned char device_addr = TCA9548A_ADDR_8BIT;
	MD_STATUS ret = MD_OK;	
	
	switch(channel)
	{
		case 1:
			ch = TCA9548A_CHANNEL_0;
			break;
		case 2:
			ch = TCA9548A_CHANNEL_1;
			break;
		case 3:
			ch = TCA9548A_CHANNEL_2;
			break;
		case 4:
			ch = TCA9548A_CHANNEL_3;
			break;
		case 5:
			ch = TCA9548A_CHANNEL_4;
			break;
		case 6:
			ch = TCA9548A_CHANNEL_5;
			break;
		case 7:
			ch = TCA9548A_CHANNEL_6;
			break;
		case 8:
			ch = TCA9548A_CHANNEL_7;
			break;
		case 9:
			ch = TCA9548A_CHANNEL_ALL;
			break;	
		default:
			// ch = 0x00;
			break;
	}


    u32TCA9548ATimeOutCnt = IIC1x_TIMEOUT_LIMIT;
    while (drv_get_IIC11_send_flag() || drv_Is_IIC11_bus_busy())
    {
        u32TCA9548ATimeOutCnt--;
        if(u32TCA9548ATimeOutCnt == 0)
        {
            printf("%s bus busy(ready for xfer),dev addr:0x%02X,reg addr:0x%02X\r\n",device_addr,ch);
            return MD_BUSY1;
        }
    }	//Make sure bus is ready for xfer
	drv_set_IIC11_send_flag(1);	
	
	ret = drv_IIC11_I2C_write(device_addr, &ch , 1);
	if (ret != MD_OK)
    {
        printf("[I2C write error1]0x%02X\r\n" , ret);
		return ret;
	} 
	
    u32TCA9548ATimeOutCnt = IIC1x_TIMEOUT_LIMIT;
	while (drv_get_IIC11_send_flag() || drv_Is_IIC11_bus_busy())
    {
        u32TCA9548ATimeOutCnt--;
        if(u32TCA9548ATimeOutCnt == 0)
        {
            printf("%s bus busy(ready for xfer),dev addr:0x%02X,reg addr:0x%02X\r\n",device_addr,ch);
            return MD_BUSY1;
        }
    } 	//Wait until the xfer is complete
   	// R_IIC11_StopCondition();
    
	// delay_ms(5);

    return ret;	
	
}


