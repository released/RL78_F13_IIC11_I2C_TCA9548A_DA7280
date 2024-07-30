/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

#include "inc_main.h"

#include "misc_config.h"
#include "custom_func.h"

#include "simple_DA7280_driver.h"

#include "rm_da7280.h"
#include "SmartCanvas_config.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_32bit flag_HAPTIC_CTL;
#define FLAG_HAPTIC_SET_TIMER_PERIOD                 	    (flag_HAPTIC_CTL.bit0)
#define FLAG_HAPTIC_TRIG_DA7280_FINISH                      (flag_HAPTIC_CTL.bit1)
#define FLAG_HAPTIC_DEMO_START                 	            (flag_HAPTIC_CTL.bit2)
#define FLAG_HAPTIC_STATE_MACHINE                           (flag_HAPTIC_CTL.bit3)
#define FLAG_HAPTIC_INIT_DA7280_RDY                         (flag_HAPTIC_CTL.bit4)
#define FLAG_HAPTIC_REVERSE5                                (flag_HAPTIC_CTL.bit5)
#define FLAG_HAPTIC_REVERSE6                                (flag_HAPTIC_CTL.bit6)
#define FLAG_HAPTIC_REVERSE7                                (flag_HAPTIC_CTL.bit7)

#define FLAG_HAPTIC_PLAY_IDX1                               (flag_HAPTIC_CTL.bit8)
#define FLAG_HAPTIC_PLAY_IDX2                               (flag_HAPTIC_CTL.bit9)
#define FLAG_HAPTIC_PLAY_IDX3                               (flag_HAPTIC_CTL.bit10)
#define FLAG_HAPTIC_PLAY_IDX4                               (flag_HAPTIC_CTL.bit11)
#define FLAG_HAPTIC_PLAY_IDX5                               (flag_HAPTIC_CTL.bit12)
#define FLAG_HAPTIC_PLAY_IDX6                               (flag_HAPTIC_CTL.bit13)
#define FLAG_HAPTIC_PLAY_IDX7                               (flag_HAPTIC_CTL.bit14)
#define FLAG_HAPTIC_PLAY_IDX8                               (flag_HAPTIC_CTL.bit15)

#define FLAG_HAPTIC_PLAY_IDX9                               (flag_HAPTIC_CTL.bit16)
#define FLAG_HAPTIC_PLAY_IDX10                              (flag_HAPTIC_CTL.bit17)
#define FLAG_HAPTIC_PLAY_IDX11                              (flag_HAPTIC_CTL.bit18)
#define FLAG_HAPTIC_PLAY_IDX12                              (flag_HAPTIC_CTL.bit19)
#define FLAG_HAPTIC_PLAY_IDX13                              (flag_HAPTIC_CTL.bit20)
#define FLAG_HAPTIC_PLAY_IDX14                              (flag_HAPTIC_CTL.bit21)
#define FLAG_HAPTIC_PLAY_IDX15                              (flag_HAPTIC_CTL.bit22)
#define FLAG_HAPTIC_PLAY_IDX0                               (flag_HAPTIC_CTL.bit23)


struct flag_8bit flag_extIRQ_CTL;
#define FLAG_DA7280A_extIRQ1                 	            (flag_extIRQ_CTL.bit0)
#define FLAG_DA7280A_extIRQ2                 	            (flag_extIRQ_CTL.bit1)
#define FLAG_DA7280A_extIRQ3                 	            (flag_extIRQ_CTL.bit2)
#define FLAG_DA7280A_extIRQ4                 	            (flag_extIRQ_CTL.bit3)
#define FLAG_DA7280A_extIRQ5                 	            (flag_extIRQ_CTL.bit4)
#define FLAG_DA7280A_extIRQ6                 	            (flag_extIRQ_CTL.bit5)
#define FLAG_DA7280A_extIRQ7                 	            (flag_extIRQ_CTL.bit6)
#define FLAG_DA7280A_extIRQ8                 	            (flag_extIRQ_CTL.bit7)


/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

extern MD_STATUS IIC11_I2C_read(unsigned char device_addr,unsigned char reg_addr,unsigned char* rx_xfer_data,unsigned short rx_num);
extern MD_STATUS IIC11_I2C_write(unsigned char device_addr,unsigned char reg_addr,unsigned char* tx_xfer_data,unsigned short tx_num);

/*
    example code : 9 mode
     SEQUENCE_SHORT         = 1,
     SEQUENCE_MEDIUM        = 2,
     SEQUENCE_LONG          = 3,
     SEQUENCE_VERY_LONG     = 4,
     SEQUENCE_SHORT_LONG    = 5,
     SEQUENCE_PHONE_RING    = 6,
     SEQUENCE_QUIET         = 7,
     SEQUENCE_RAMP_UP       = 8,
     SEQUENCE_ALARM_CLOCK,


    SEQUENCE_SHORT          1 Short,        constant buzz (40ms)
    SEQUENCE_MEDIUM         2 Medium,       constant buzz (140ms)
    SEQUENCE_LONG           3 Long,         constant buzz (200ms)
    SEQUENCE_VERY_LONG      4               Very long, constant buzz (450ms) 
    SEQUENCE_SHORT_LONG     5               Short buzz, followed by long buzz (600ms)
    SEQUENCE_PHONE_RING     6               2 short buzzes, followed by long buzz (800ms)
    SEQUENCE_QUIET          7               Low amplitude buzz that reduces in intensity (500ms)
    SEQUENCE_RAMP_UP        8               Gradually increasing buzz from silent (500ms)
    SEQUENCE_ALARM_CLOCK    9               Sequence of 9 short buzzes (1200ms)

*/

const unsigned char snp_mem[100] = {
    0x08, 0x0B, 0x15, 0x16, 0x19, 0x1B, 0x1F, 0x21, 0x22, 0x23,
    0x29, 0x31, 0x32, 0x35, 0x38, 0x40, 0x4A, 0x4C, 0x4F, 0x51,
    0x5B, 0x27, 0x17, 0x04, 0xB3, 0xD0, 0xF2, 0xB5, 0x77, 0x39,
    0x70, 0x70, 0x79, 0x69, 0x70, 0x79, 0x09, 0x8C, 0x75, 0x08,
    0x84, 0x73, 0x01, 0x9C, 0x74, 0x00, 0x81, 0x08, 0x84, 0x73,
    0x0D, 0x1A, 0x88, 0x06, 0x1A, 0xA0, 0x06, 0x09, 0x88, 0x06,
    0x10, 0x90, 0x1A, 0xA0, 0x06, 0x0D, 0x10, 0x90, 0x0D, 0x10,
    0x90, 0x12, 0xB0, 0x06, 0x18, 0x1B, 0x18, 0x1C, 0x06, 0x18,
    0x0D, 0xB8, 0x12, 0xD0, 0x06, 0x10, 0xA0, 0x05, 0x10, 0xA0,
    0x05, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char DA7280_nIRQ_mapping_to_INT(unsigned char nIRQ)
{
    unsigned char ext_PinNo = 0;

    switch(nIRQ)
    {
        case 1:
            ext_PinNo = 1;
            break;
        case 2:
            ext_PinNo = 4;
            break;
        case 3:
            ext_PinNo = 0;
            break;
        case 4:
            ext_PinNo = 2;
            break;
        case 5:
            ext_PinNo = 3;
            break;
        case 6:
            ext_PinNo = 11;
            break;
        case 7:
            ext_PinNo = 10;
            break;
        case 8:
            ext_PinNo = 5;
            break;
    }
    
    return ext_PinNo;
}

/*
    GPIO0:P13
    GPIO1:P46
    GPIO2:P45

    nIRQ1:P125/INTP1
    nIRQ2:P120/INTP4
    nIRQ3:P137/INTP0
    nIRQ4:P30/INTP2

    nIRQ5:P17/INTP3
    nIRQ6:P51/INTP11
    nIRQ7:P53/INTP10
    nIRQ8:P12/INTP5

*/

void clr_DA7280_trig_nIRQ(unsigned char nIRQ)
{
    unsigned char ext_PinNo = 0;

    ext_PinNo = DA7280_nIRQ_mapping_to_INT(nIRQ);

    switch(ext_PinNo)
    {
        case 1:
            FLAG_DA7280A_extIRQ1 = 0;
            break;
        case 4:
            FLAG_DA7280A_extIRQ2 = 0;
            break;
        case 0:
            FLAG_DA7280A_extIRQ3 = 0;
            break;
        case 2:
            FLAG_DA7280A_extIRQ4 = 0;
            break;

        case 3:
            FLAG_DA7280A_extIRQ5 = 0;
            break;
        case 11:
            FLAG_DA7280A_extIRQ6 = 0;
            break;
        case 10:
            FLAG_DA7280A_extIRQ7 = 0;
            break;
        case 5:
            FLAG_DA7280A_extIRQ8 = 0;
            break;
    }
}


unsigned char read_DA7280_trig_nIRQ(unsigned char nIRQ)
{
    unsigned char res = 0;
    unsigned char ext_PinNo = 0;

    ext_PinNo = DA7280_nIRQ_mapping_to_INT(nIRQ);

    switch(ext_PinNo)
    {
        case 1:
            res = FLAG_DA7280A_extIRQ1;
            break;
        case 4:
            res = FLAG_DA7280A_extIRQ2;
            break;
        case 0:
            res = FLAG_DA7280A_extIRQ3;
            break;
        case 2:
            res = FLAG_DA7280A_extIRQ4;
            break;

        case 3:
            res = FLAG_DA7280A_extIRQ5;
            break;
        case 11:
            res = FLAG_DA7280A_extIRQ6;
            break;
        case 10:
            res = FLAG_DA7280A_extIRQ7;
            break;
        case 5:
            res = FLAG_DA7280A_extIRQ8;
            break;
    }

    return res;
}

void DA7280_trig_nIRQ(unsigned char nIRQ)
{
    unsigned char ext_PinNo = nIRQ;

    switch(ext_PinNo)
    {
        case 1:
            FLAG_DA7280A_extIRQ1 = 1;
            break;
        case 4:
            FLAG_DA7280A_extIRQ2 = 1;
            break;
        case 0:
            FLAG_DA7280A_extIRQ3 = 1;
            break;
        case 2:
            FLAG_DA7280A_extIRQ4 = 1;
            break;

        case 3:
            FLAG_DA7280A_extIRQ5 = 1;
            break;
        case 11:
            FLAG_DA7280A_extIRQ6 = 1;
            break;
        case 10:
            FLAG_DA7280A_extIRQ7 = 1;
            break;
        case 5:
            FLAG_DA7280A_extIRQ8 = 1;
            break;
    }
}

MD_STATUS _simple_DA7280_Check_All_IRQ_EVENT(void)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = 0;
    const unsigned char indicator[] = "DA7280_CheckIRQ_MASK";

    tmp = 0;
    reg_addr = DA7280_IRQ_MASK1;
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-IRQ_MASK1-r:0x%02X\r\n",indicator,tmp);


    tmp = 0;
    reg_addr = DA7280_IRQ_EVENT_ACTUATOR_FAULT;
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-IRQ_EVENT_ACTUATOR_FAULT-r:0x%02X\r\n",indicator,tmp);

    tmp = 0;
    reg_addr = DA7280_IRQ_STATUS2;
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-IRQ_STATUS2-r:0x%02X\r\n",indicator,tmp);

    tmp = 0;
    reg_addr = DA7280_IRQ_MASK2;
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-IRQ_MASK2-r:0x%02X\r\n",indicator,tmp);


    // tmp = (unsigned char)(0x00 & ~DA7280_SEQ_DONE_M_MASK);
    // tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_SEQ_DONE_M_SHIFT));  

    // IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    // printf("%s-2-w:0x%02X\r\n",indicator,tmp);
	return ret;
}

MD_STATUS _simple_DA7280_ForceClearSeqDone(void)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_MASK1;
    unsigned char SEQ_DONE_M  = 0;
    const unsigned char indicator[] = "DA7280_ForceClearSeqDone";
    bool enable = 1;

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_DONE_M = tmp;
    printf("%s-1-r:0x%02X\r\n",indicator,SEQ_DONE_M);

    tmp = (unsigned char)(0x00 & ~DA7280_SEQ_DONE_M_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_SEQ_DONE_M_SHIFT));  

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_CheckIRQ_EVENT_WARNING_DIAG(void)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_EVENT_WARNING_DIAG;
    const unsigned char indicator[] = "DA7280_CheckIRQ_EVENT_WARNING_DIAG";
 
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s-1-r:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-1-r:0x%02X\r\n",indicator,tmp);

    switch(tmp)
    {
        case DA7280_E_OVERTEMP_WARN_MASK:
            printf("%s-2:%s\r\n",indicator,"E_OVERTEMP_WARN");
            break;
        case DA7280_E_MEM_TYPE_MASK:
            printf("%s-2:%s\r\n",indicator,"E_MEM_TYPE");
            break;
        case DA7280_E_LIM_DRIVE_ACC_MASK:
            printf("%s-2:%s\r\n",indicator,"E_LIM_DRIVE_ACC");
            break;
        case DA7280_E_LIM_DRIVE_MASK:
            printf("%s-2:%s\r\n",indicator,"E_LIM_DRIVE");
            break;
        default:
            printf("[error]%s-2:0x%02X\r\n",indicator,tmp);
            break;
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }

	return ret;
}



MD_STATUS _simple_DA7280_CheckIRQ_EVENT_SEQ_DIAG(void)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_EVENT_PAT_DIAG;
    const unsigned char indicator[] = "DA7280_CheckIRQ_EVENT_SEQ_DIAG";
 
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s-1-r:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-1-r:0x%02X\r\n",indicator,tmp);

    switch(tmp)
    {
        case DA7280_E_PWM_FAULT_MASK:
            printf("%s-2:%s\r\n",indicator,"E_PWM_FAULT");
            break;
        case DA7280_E_MEM_FAULT_MASK:
            printf("%s-2:%s\r\n",indicator,"E_MEM_FAULT");
            break;
        case DA7280_E_SEQ_ID_FAULT_MASK:
            printf("%s-2:%s\r\n",indicator,"E_SEQ_ID_FAULT");
            break;
        default:
            printf("[error]%s-2:0x%02X\r\n",indicator,tmp);
            break;
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }

	return ret;
}


MD_STATUS _simple_DA7280_SetIRQEvent1(unsigned char report_event)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_EVENT1;
    unsigned char event = 0;
    const unsigned char indicator[] = "DA7280_SetIRQEvent1";
 
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    event = tmp;
    
    #if (ENABLE_DA7280_IRQ_EVENT_LOG == 1)
    printf("%s-1-r:0x%02X\r\n",indicator,event);
    #endif

    tmp = (unsigned char)(event & ~report_event);
    tmp = (unsigned char)(tmp | report_event );  

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);


    #if (ENABLE_DA7280_IRQ_EVENT_LOG == 1)
    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
    #endif

    //  read back event status  
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    #if (ENABLE_DA7280_IRQ_EVENT_LOG == 1)
    printf("%s-3-r:0x%02X\r\n",indicator,tmp);
    #endif

	return ret;
}


MD_STATUS _simple_DA7280_CheckIRQEvent1(unsigned char* report_event)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_EVENT1;
    const unsigned char indicator[] = "DA7280_CheckIRQEvent1";
 
    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    if (!tmp)
    {
        *report_event = HAPTIC_SUCCESS;
	    return ret;
    }

    switch(tmp)
    {
        case E_SEQ_CONTINUE:
            *report_event = E_SEQ_CONTINUE;
            break;
        case E_UVLO:
            *report_event = E_UVLO;
            break;
        case E_SEQ_DONE:
            *report_event = E_SEQ_DONE;
            break;
        case E_OVERTEMP_CRIT:
            *report_event = E_OVERTEMP_CRIT;
            break;
        case E_SEQ_FAULT:
            *report_event = E_SEQ_FAULT;
            break;
        case E_WARNING:
            *report_event = E_WARNING;
            break;
        case E_ACTUATOR_FAULT:
            *report_event = E_ACTUATOR_FAULT;
            break;
        case E_OC_FAULT:
            *report_event = E_OC_FAULT;
            break;
    }

    if (ret == MD_OK)
    {
        #if (ENABLE_DA7280_IRQ_EVENT_LOG == 1)
        printf("%s done(0x%02X)\r\n",indicator,tmp);
        #endif
    }
    else
    {
        #if (ENABLE_DA7280_IRQ_EVENT_LOG == 1)
        printf("%s ERROR(0x%02X:0x%02X)\r\n",indicator,tmp,*report_event);
        #endif
    }

	return ret;
}



MD_STATUS _simple_DA7280_SetSequenceDone(bool enable)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_MASK1;
    unsigned char SEQ_DONE_M  = 0;
    const unsigned char indicator[] = "DA7280_SetSequenceDone";

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_DONE_M = tmp;
    printf("%s-1-r:0x%02X\r\n",indicator,SEQ_DONE_M);

    tmp = (unsigned char)(SEQ_DONE_M & ~DA7280_SEQ_DONE_M_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_SEQ_DONE_M_SHIFT));  

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_CheckSequenceDone(void)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_MASK1;
    unsigned char res = 0;
    const unsigned char indicator[] = "DA7280_CheckSequenceDone";

    do
    {
        
        ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
            return ret;
        }
        /*
            SEQ_DONE_M 
            1 : complete ( nIRQ = 0)
        */
        res =  (tmp >> DA7280_SEQ_DONE_M_SHIFT) & 0x01 ? 1 : 0 ; 

    } while (res != 1);    

	return ret;
}

/*
    STANDBY_EN 
    1 : stand by
    0 : idle
*/
MD_STATUS _simple_DA7280_SetStandByState(bool enable)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CTL1;
    unsigned char STANDBY_EN  = 0;
    const unsigned char indicator[] = "DA7280_SetStandByState";

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    STANDBY_EN = tmp;
    printf("%s-1-r:0x%02X\r\n",indicator,STANDBY_EN);

    tmp = (unsigned char)(STANDBY_EN & ~DA7280_STANDBY_EN_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_STANDBY_EN_SHIFT));  

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_SetPlayBackMemory(bool enable)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CTL1;
    unsigned char SEQ_START  = 0;
    const unsigned char indicator[] = "DA7280_SetPlayBackMemory";

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_START = tmp;
    #if (ENABLE_DA7280_LOG == 1)
    printf("%s-1-r:0x%02X\r\n",indicator,SEQ_START);
    #endif

    tmp = (unsigned char)(SEQ_START & ~DA7280_SEQ_START_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_SEQ_START_SHIFT));  

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    #if (ENABLE_DA7280_LOG == 1)
    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
    #endif
	return ret;
}


MD_STATUS _simple_DA7280_Set_PS_SEQ_ID_LOOP(unsigned char SEQ_ID , unsigned char SEQ_LOOP)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_SEQ_CTL2;
    unsigned char SEQ_CTL2  = 0;
    const unsigned char indicator[] = "DA7280_Set_PS_SEQ_ID_LOOP";

    if (SEQ_ID > 15)
    {
        return MD_ARGERROR;
    }
    if (SEQ_LOOP > 15)
    {
        return MD_ARGERROR;
    }

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_CTL2 = tmp;
    #if (ENABLE_DA7280_LOG == 1)
    printf("%s-1-r:0x%02X\r\n",indicator,SEQ_CTL2);
    #endif

    // tmp = (unsigned char)(tmp | (SEQ_ID << DA7280_PS_SEQ_ID_SHIFT));  
    
    tmp = (unsigned char)(0x00 | (SEQ_ID << DA7280_PS_SEQ_ID_SHIFT));   // force to renew ID,loop each time
    tmp = (unsigned char)(tmp | (SEQ_LOOP << DA7280_PS_SEQ_LOOP_SHIFT));    // 0 ~ 0XF

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    #if (ENABLE_DA7280_LOG == 1)
    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
    #endif
	return ret;
}


MD_STATUS _simple_DA7280_SetContinueSequence(bool enable)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_SEQ_CTL1;
    unsigned char SEQ_CONTINUE  = 0;
    const unsigned char indicator[] = "DA7280_SetContinueSequence";

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_CONTINUE = tmp;
    #if (ENABLE_DA7280_LOG == 1)
    printf("%s-1-r:0x%02X\r\n",indicator,SEQ_CONTINUE);
    #endif

    tmp = (unsigned char)(SEQ_CONTINUE & ~DA7280_SEQ_CONTINUE_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_SEQ_CONTINUE_SHIFT));  

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    #if (ENABLE_DA7280_LOG == 1)
    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
    #endif
	return ret;
}


MD_STATUS _simple_DA7280_PlayFromMemory(void)
{
    /*
        playFromMemory
        - read DA7280_TOP_CTL1 (1 byte)
        - write DA7280_TOP_CTL1 (1 byte)
            transfer_data[1] = (uint8_t)(playMem & ~DA7280_SEQ_START_MASK);
            transfer_data[1] = (uint8_t)(transfer_data[1] | DA7280_SEQ_START_MASK);
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CTL1;
    unsigned char playMem = 0;
    const unsigned char indicator[] = "DA7280_PlayFromMemory";

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    playMem = tmp;
    printf("%s-1-r:0x%02X\r\n",indicator,playMem);

    tmp = (unsigned char)(playMem & ~DA7280_SEQ_START_MASK);
    tmp = (unsigned char)(tmp | DA7280_SEQ_START_MASK);

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
	return ret;
}

MD_STATUS _simple_DA7280_WriteWaveform(const unsigned char*  buffer_mem)
{
    /*
        setWaveform : snp_mem
            - DA7280_SNP_MEM_0 , with seq write ( snp_mem : 100 bytes )
                ex : addr: DA7280_SNP_MEM_0+i , data : snp_mem[i]
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_SNP_MEM_0;
    unsigned char i = 0;
    const unsigned char indicator[] = "DA7280_WriteWaveform";

    for( i = 0 ; i < 100 ; i++)
    {
        tmp = buffer_mem[i];
        ret = IIC11_I2C_write(device_addr,reg_addr + i, &tmp ,1);
        
        #if 1   // debug
        printf("0x%02X," , tmp);
        if ((i+1)%10 ==0)
        {
            printf("\r\n");
        }   
        #endif

        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
            return ret;
        }
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }

	return ret;
}

MD_STATUS _simple_DA7280_SetMode(unsigned char mode)
{
    /*
        SetMode : DA7280_ETWM_MODE
        - read DA7280_TOP_CTL1 (1 byte)
        - write DA7280_TOP_CTL1 (1 byte)
            transfer_data[1] = (uint8_t)(modeSet & ~DA7280_OPERATION_MODE_MASK);
            transfer_data[1] = (uint8_t)(transfer_data[1] | (mode << DA7280_OPERATION_MODE_SHIFT));
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CTL1;
    unsigned char modeSet = 0;
    const unsigned char indicator[] = "DA7280_SetMode";

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    modeSet = tmp;
    printf("%s-1-r:0x%02X\r\n",indicator,modeSet);

    tmp = (unsigned char)(modeSet & ~DA7280_OPERATION_MODE_MASK);
    tmp = (unsigned char)(tmp | (mode << DA7280_OPERATION_MODE_SHIFT));

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_SetFrequencyTracking(bool enable)
{
    /*  
        SetFrequencyTracking : enable
        - read DA7280_TOP_CFG1 (1 byte)
        - write DA7280_TOP_CFG1 (1 byte)
            transfer_data[1] = (uint8_t)(freqTrack & ~DA7280_FREQ_TRACK_EN_MASK);
            transfer_data[1] = (uint8_t)(transfer_data[1] | ((enable ? 1:0) << DA7280_FREQ_TRACK_EN_SHIFT));  
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CFG1;
    unsigned char freqTrack = 0;
    const unsigned char indicator[] = "DA7280_SetFrequencyTracking";

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    freqTrack = tmp;
    printf("%s-1-r:0x%02X\r\n",indicator,freqTrack);

    tmp = (unsigned char)(freqTrack & ~DA7280_FREQ_TRACK_EN_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_FREQ_TRACK_EN_SHIFT));  

    IIC11_I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2-w:0x%02X\r\n",indicator,tmp);
	return ret;
}

MD_STATUS _simple_DA7280_Open(void)
{
    /*
        Open : 
        - read DA7280_CHIP_REV (1 byte) , check if return value is 0xBA  (1 byte)  
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_CHIP_REV;
    const unsigned char indicator[] = "DA7280_Open";

    ret = IIC11_I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s:0x%02X\r\n", indicator ,tmp);
	return ret;
}

/*
    data len : * // 155 , 142
    set reg with data
*/

MD_STATUS _simple_DA7280_SetConfigTbl(unsigned char data_len, const unsigned char  buffer_tbl[][2])
{

	MD_STATUS ret = MD_OK;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = 0;
    unsigned char reg_data = 0;
    const unsigned char indicator[] = "DA7280_SetConfigTbl";
    unsigned char i = 0;

    printf("table size:%2d\r\n", data_len );

    for( i = 0 ; i < data_len ; i++)
    {
        reg_addr = buffer_tbl[i][0];
        reg_data = buffer_tbl[i][1];
        ret = IIC11_I2C_write(device_addr,reg_addr, &reg_data ,1);
        
        #if 0   // debug
        printf("(%3d)addr:0x%02X,data:0x%02X\r\n" ,i, reg_addr,reg_data);

        // printf("0x%02X," , tmp);
        // if ((i+1)%10 ==0)
        // {
        //     printf("\r\n");
        // }   
        #endif

        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
            return ret;
        }
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }

	return ret;
}

void DA7280_forceStop(void)
{
    /*
        At any time during operation in RTWM mode, set OPERATION_MODE or SEQ_START = 0 to return 
        to the IDLE or STANDBY state. 
    */
   
    MD_STATUS ret = MD_OK;
    const unsigned char indicator[] = "DA7280_forceStop";

    ret = _simple_DA7280_SetPlayBackMemory(0);

    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }
}


MD_STATUS DA7280_PlaybackFinishCheck(void)
{
    /*    
        4.  When the haptic sequence is completed, DA7280 will signal this by setting nIRQ = 0 and setting 
        SEQ_DONE_M = 1. DA7280 will then return to IDLE or STANDBY state, depending on the value 
        of STANDBY_EN.     
    */

    MD_STATUS ret = MD_OK;
    const unsigned char indicator[] = "DA7280_PlaybackFinishCheck";
    unsigned char event = 0;

    do
    {
        ret = _simple_DA7280_CheckIRQEvent1(&event);
        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
        }

        if ((event == E_SEQ_DONE) || 
            (event == HAPTIC_SUCCESS))
        {
            /*
                5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
            */
            ret = _simple_DA7280_SetIRQEvent1(event);
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }   

            break;
        }
        else
        {
            switch(event)
            {
                case E_SEQ_FAULT:
                    _simple_DA7280_CheckIRQ_EVENT_SEQ_DIAG();
                    printf("[error]%s:event-E_SEQ_FAULT(0x%02X)\r\n",indicator,event);
                    break;

                case E_WARNING:
                    _simple_DA7280_CheckIRQ_EVENT_WARNING_DIAG();
                    printf("[error]%s:event-E_WARNING(0x%02X)\r\n",indicator,event);
                    break;

                case E_SEQ_CONTINUE:
                    printf("[error]%s:event-E_SEQ_CONTINUE(0x%02X)\r\n",indicator,event);
                    break;
                case E_UVLO:
                    printf("[error]%s:event-E_UVLO(0x%02X)\r\n",indicator,event);
                    break;
                case E_OVERTEMP_CRIT:
                    printf("[error]%s:event-E_OVERTEMP_CRIT(0x%02X)\r\n",indicator,event);
                    break;
                case E_ACTUATOR_FAULT:
                    printf("[error]%s:event-E_ACTUATOR_FAULT(0x%02X)\r\n",indicator,event);
                    break;
                case E_OC_FAULT:
                    printf("[error]%s:event-E_OC_FAULT(0x%02X)\r\n",indicator,event);
                    break;

                default :                    
                    printf("[error]%s:0x%02X-EXCEPTION(0x%02X)\r\n",indicator ,event);
                    break;
            }

            ret = _simple_DA7280_Check_All_IRQ_EVENT();
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }

            /*
                5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
            */
            ret = _simple_DA7280_SetIRQEvent1(event);
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }

            break;
        }
        #if 0
        else if (event == E_SEQ_FAULT)
        {
            _simple_DA7280_CheckIRQ_EVENT_SEQ_DIAG();
            printf("[error]%s:event-E_SEQ_FAULT\r\n",indicator);
            
            /*
                5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
            */
            ret = _simple_DA7280_SetIRQEvent1(event);
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }
        }
        else if (event == E_WARNING)
        {
            _simple_DA7280_CheckIRQ_EVENT_WARNING_DIAG();
            printf("[error]%s:event-E_WARNING\r\n",indicator);
            
            /*
                5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
            */
            ret = _simple_DA7280_SetIRQEvent1(event);
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }
        }
        else
        {

            printf("[error]%s:event-0x%02X-",indicator,event);
            switch(event)
            {
                case E_SEQ_CONTINUE:
                    printf("E_SEQ_CONTINUE\r\n");
                    break;
                case E_UVLO:
                    printf("E_UVLO\r\n");
                    break;
                case E_OVERTEMP_CRIT:
                    printf("E_OVERTEMP_CRIT\r\n");
                    break;
                case E_ACTUATOR_FAULT:
                    printf("E_ACTUATOR_FAULT\r\n");
                    break;
                case E_OC_FAULT:
                    printf("E_OC_FAULT\r\n");
                    break;
            }
            
            /*
                5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
            */
            ret = _simple_DA7280_SetIRQEvent1(event);
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }

        }
        #endif

    }while(1);

    // /*
    //     5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
    // */
    // ret = _simple_DA7280_SetIRQEvent1(event);
    // if (ret != MD_OK)
    // {
    //     printf("[error]%s:0x%02X\r\n",indicator,ret);
    // }    

    if (ret == MD_OK)
    {
        #if (ENABLE_DA7280_LOG == 1)
        printf("%s(0x%02X) done\r\n",indicator,event);
        #endif
    }

    return ret;
}

void DA7280_PlaybackIndex(unsigned char idx , unsigned char loop , unsigned char enableContinue)
{

    MD_STATUS ret = MD_OK;
    const unsigned char indicator[] = "DA7280_PlaybackIndex";
    // unsigned char event = 0;

    #if (ENABLE_DA7280_LOG == 1)
    printf("\r\n%s start,seq:%d\r\n",indicator,idx);
    #endif

    /*
        1.  While in the IDLE or STANDBY state, configure PS_SEQ_ID and PS_SEQ_LOOP to select the 
        desired sequence from Waveform Memory.  
    */
    ret = _simple_DA7280_Set_PS_SEQ_ID_LOOP(idx,loop);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }

    /*
        2.  For first-time playback, set OPERATION_MODE = 3. On subsequent sequence playbacks, this 
        step can be skipped (if OPERATION_MODE = 3). The haptic sequence will not begin playing 
        until a start event is created by setting SEQ_START = 1.     
    */
    ret = _simple_DA7280_SetPlayBackMemory(1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }

    /*
        3.  While in the DRIVE state, set SEQ_CONTINUE = 1 to repeat the sequence.     
    */
    ret = _simple_DA7280_SetContinueSequence(enableContinue);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }

    #if 0
    /*    
        4.  When the haptic sequence is completed, DA7280 will signal this by setting nIRQ = 0 and setting 
        SEQ_DONE_M = 1. DA7280 will then return to IDLE or STANDBY state, depending on the value 
        of STANDBY_EN.     
    */


    do
    {
        ret = _simple_DA7280_CheckIRQEvent1(&event);
        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
        }
    }while(event != E_SEQ_DONE);

    /*
        5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
    */
    ret = _simple_DA7280_SetIRQEvent1(event);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }
    #endif

    if (ret == MD_OK)
    {
        
        #if (ENABLE_DA7280_LOG == 1)
        printf("%s done\r\n",indicator);
        #endif
    }
}

/*
    SCL: P71
    SDA: P70
    nIRQ : P77 (INTP12)
*/

void DA7280_init(void)  //Byte 1: Defines the number of sequences stored.
{
    unsigned char data_len = 0;
    unsigned char res = 0;

    #if 1   // use config table directly
        
    data_len = sizeof(ALPS_AFR14A901B_Sine)/(sizeof(ALPS_AFR14A901B_Sine[0]));
    res = _simple_DA7280_SetConfigTbl(data_len,ALPS_AFR14A901B_Sine);   

    #else
    res = _simple_DA7280_Open();
    res = _simple_DA7280_SetFrequencyTracking(true);
    //res =  _simple_DA7280_SetMode(DA7280_ETWM_MODE);    // DA7280_RTWM_MODE ? 
    res = _simple_DA7280_SetMode(DA7280_RTWM_MODE);
    res = _simple_DA7280_WriteWaveform(snp_mem);
    #endif

    if (res != MD_OK)
    {
        DA7280_set_Init_Flag(0);
    }
    else
    {
        DA7280_set_Init_Flag(1);
    }

    printf("DA7280_init end (0x%02X)\r\n" , res);
}


void DA7280_key_filter(unsigned char key)
{
    if (flag_play)
    {
        flag_play = 0;
        switch(key)
        {
            case '0':
                FLAG_HAPTIC_PLAY_IDX0 = 1;
                break;
            case '1':
                FLAG_HAPTIC_PLAY_IDX1 = 1;
                break;
            case '2':
                FLAG_HAPTIC_PLAY_IDX2 = 1;
                break;
            case '3':
                FLAG_HAPTIC_PLAY_IDX3 = 1;
                break;
            case '4':
                FLAG_HAPTIC_PLAY_IDX4 = 1;
                break;
            case '5':
                FLAG_HAPTIC_PLAY_IDX5 = 1;
                break;
            case '6':
                FLAG_HAPTIC_PLAY_IDX6 = 1;
                break;
            case '7':
                FLAG_HAPTIC_PLAY_IDX7 = 1;
                break;
            case '8':
                FLAG_HAPTIC_PLAY_IDX8 = 1;
                break;
            case '9':
                FLAG_HAPTIC_PLAY_IDX9 = 1;
                break;
            case 'A':
            case 'a':
                FLAG_HAPTIC_PLAY_IDX10 = 1;
                break;
            case 'B':
            case 'b':
                FLAG_HAPTIC_PLAY_IDX11 = 1;
                break;
            case 'C':
            case 'c':
                FLAG_HAPTIC_PLAY_IDX12 = 1;
                break;
            case 'D':
            case 'd':
                FLAG_HAPTIC_PLAY_IDX13 = 1;
                break;
            case 'E':
            case 'e':
                FLAG_HAPTIC_PLAY_IDX14 = 1;
                break;
            case 'F':
            case 'f':
                FLAG_HAPTIC_PLAY_IDX15 = 1;
                break;
        }
    }
}


void DA7280_playback(void)
{
    if (FLAG_HAPTIC_PLAY_IDX0)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX0 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX0 = 0;
    }

    if (FLAG_HAPTIC_PLAY_IDX1)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX1 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX1 = 0;
    }

    if (FLAG_HAPTIC_PLAY_IDX2)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX2 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX2 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX3)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX3 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX3 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX4)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX4 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX4 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX5)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX5 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX5 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX6)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX6 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX6 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX7)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX7 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX7 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX8)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX8 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX8 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX9)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX9 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX9 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX10)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX10 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX10 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX11)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX11 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX11 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX12)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX12 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX12 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX13)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX13 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX13 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX14)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX14 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX14 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX15)
    {
        DA7280_PlaybackIndex(SEQUENCE_IDX15 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX15 = 0;
    }

}

void DA7280_Process(unsigned char key)
{    
    if (!DA7280_is_Init_Ready())
    {
        // init fail
        return;
    }

    // init OK , continne process
    #if 0
    if (!DA7280_is_demo_mode_enable())
    {
        DA7280_key_filter(key);
        DA7280_playback();
    }
    else
    {        
        DA7280_demo_mode();
    }
    #endif

    if (FLAG_HAPTIC_TRIG_DA7280_FINISH)
    {
        FLAG_HAPTIC_TRIG_DA7280_FINISH = 0;
        DA7280_PlaybackFinishCheck();
        printf("DA7280 exec finish\r\n\r\n");
    }

}

void DA7280_set_Init_Flag(bool flag)
{
    FLAG_HAPTIC_INIT_DA7280_RDY = flag;
}

bool DA7280_is_Init_Ready(void)
{
    return FLAG_HAPTIC_INIT_DA7280_RDY;
}

void DA7280_nIRQ_Process_in_IRQ(void)
{
    FLAG_HAPTIC_TRIG_DA7280_FINISH = 1;
}

void DA7280_set_TIMER_PERIOD(void)
{
    FLAG_HAPTIC_SET_TIMER_PERIOD = 1;
}

void DA7280_reset_TIMER_PERIOD(void)
{
    FLAG_HAPTIC_SET_TIMER_PERIOD = 0;
}

bool DA7280_is_TIMER_PERIOD_Trig(void)
{
    return FLAG_HAPTIC_SET_TIMER_PERIOD;
}

void DA7280_demo_mode_statemachine(void)
{
    static unsigned char state = 0;
    static unsigned char loop = 0;

    printf("\r\ndemo mode seq %d(%d)\r\n" ,state ,loop);

    switch(state)
    {
        case 0:        
            DA7280_PlaybackIndex(SEQUENCE_IDX0 , 0 ,0);
            loop++;
            if (loop > 1)
            {
                loop = 0;
                state++;
            }
            break;
        case 1:        
            DA7280_PlaybackIndex(SEQUENCE_IDX1 , 0 ,0);
            loop++;
            if (loop > 1)
            {
                loop = 0;
                state++;
            }
            break;
        case 2:        
            DA7280_PlaybackIndex(SEQUENCE_IDX2 , 0 ,0);
            loop++;
            if (loop > 1)
            {
                loop = 0;
                state++;
            }
            break;
        case 3:        
            DA7280_PlaybackIndex(SEQUENCE_IDX3 , 0 ,0);
            loop++;
            if (loop > 1)
            {
                loop = 0;
                state = 0;
            }
            break;
    }

}

bool DA7280_is_demo_mode_enable(void)
{
    return FLAG_HAPTIC_DEMO_START;
}

void DA7280_demo_enable(unsigned char on)
{
    FLAG_HAPTIC_DEMO_START = ~on;
    
    if (FLAG_HAPTIC_DEMO_START)
    {
        printf("DA7280 demo mode enable\r\n");
    }
    else
    {
        printf("DA7280 demo mode disable\r\n");
    }
}

void DA7280_demo_mode(void)
{
    if (FLAG_HAPTIC_DEMO_START)
    {
        if (DA7280_is_TIMER_PERIOD_Trig())
        {
            DA7280_reset_TIMER_PERIOD();
            FLAG_HAPTIC_STATE_MACHINE = 1;
        }

        if (FLAG_HAPTIC_STATE_MACHINE)
        {
            FLAG_HAPTIC_STATE_MACHINE = 0;
            DA7280_demo_mode_statemachine();
        }
    }
    else
    {
        DA7280_reset_TIMER_PERIOD();
        FLAG_HAPTIC_STATE_MACHINE = 0;
    }
}

