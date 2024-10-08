/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

#include "inc_main.h"

#include "misc_config.h"
#include "custom_func.h"

#include "rm_da7280.h"
#include "simple_DA7280_driver.h"

#include "TCA9548A_driver.h"
/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_32bit flag_PROJ_CTL;
#define FLAG_PROJ_TIMER_PERIOD_1000MS                 	(flag_PROJ_CTL.bit0)
#define FLAG_PROJ_REVERSE1                       	    (flag_PROJ_CTL.bit1)
#define FLAG_PROJ_REVERSE2                 	            (flag_PROJ_CTL.bit2)
#define FLAG_PROJ_REVERSE3                              (flag_PROJ_CTL.bit3)
#define FLAG_PROJ_REVERSE4                              (flag_PROJ_CTL.bit4)
#define FLAG_PROJ_REVERSE5                              (flag_PROJ_CTL.bit5)
#define FLAG_PROJ_REVERSE6                              (flag_PROJ_CTL.bit6)
#define FLAG_PROJ_REVERSE7                              (flag_PROJ_CTL.bit7)


struct flag_16bit flag_BTN_CTL;
#define FLAG_BTN_1_SHORT_PRESSED                 	    (flag_BTN_CTL.bit0)
#define FLAG_BTN_2_SHORT_PRESSED                 	    (flag_BTN_CTL.bit1)
#define FLAG_BTN_3_SHORT_PRESSED                 	    (flag_BTN_CTL.bit2)
#define FLAG_BTN_4_SHORT_PRESSED                 	    (flag_BTN_CTL.bit3)
#define FLAG_BTN_5_SHORT_PRESSED                 	    (flag_BTN_CTL.bit4)
#define FLAG_BTN_6_SHORT_PRESSED                 	    (flag_BTN_CTL.bit5)
#define FLAG_BTN_7_SHORT_PRESSED                 	    (flag_BTN_CTL.bit6)
#define FLAG_BTN_8_SHORT_PRESSED                 	    (flag_BTN_CTL.bit7)
#define FLAG_BTN_1_LONG_PRESSED                 	    (flag_BTN_CTL.bit8)
#define FLAG_BTN_2_LONG_PRESSED                 	    (flag_BTN_CTL.bit9)
#define FLAG_BTN_3_LONG_PRESSED                 	    (flag_BTN_CTL.bit10)
#define FLAG_BTN_4_LONG_PRESSED                 	    (flag_BTN_CTL.bit11)
#define FLAG_BTN_5_LONG_PRESSED                 	    (flag_BTN_CTL.bit12)
#define FLAG_BTN_6_LONG_PRESSED                 	    (flag_BTN_CTL.bit13)
#define FLAG_BTN_7_LONG_PRESSED                 	    (flag_BTN_CTL.bit14)
#define FLAG_BTN_8_LONG_PRESSED                 	    (flag_BTN_CTL.bit15)

/*_____ D E F I N I T I O N S ______________________________________________*/

volatile unsigned long counter_tick = 0;
volatile unsigned long btn_counter_tick = 0;

unsigned char key_buffer = 0;
unsigned char flag_play = 0;

#define BTN_PRESSED_LONG                                (2500)

#define DEBOUNCE_INTERVAL_TIME                          (10) 
#define DEBOUNCE_SHORT_TIME                             (80)            //Press and release button settling time ( ms )
#define DEBOUNCE_LONG_TIME                              (1500)          //Press and release button settling time ( ms )

typedef struct BtnEvent_t
{
    unsigned char ButtonState;
    unsigned char ButtonShortPressed;
    unsigned char ButtonLongPressed;
    unsigned char ButtonDebounceState;
    unsigned char ButtonBitShift;
    unsigned long ButtonDebounceCnt;
    unsigned long ButtonDebounceTimer;
} BtnEvent_t;

typedef enum  
{
    BTN_IDX_1 = 0,
    BTN_IDX_2,
    BTN_IDX_3,
    BTN_IDX_4,
    BTN_IDX_5,
    BTN_IDX_6,
    BTN_IDX_7,
    BTN_IDX_8,
}BTN_IDX_t;

BtnEvent_t event_btn[8] = {0};

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/


unsigned long btn_get_tick(void)
{
	return (btn_counter_tick);
}

void btn_set_tick(unsigned long t)
{
	btn_counter_tick = t;
}

void btn_tick_counter(void)
{
	btn_counter_tick++;
    if (btn_get_tick() >= 60000)
    {
        btn_set_tick(0);
    }
}

unsigned long get_tick(void)
{
	return (counter_tick);
}

void set_tick(unsigned long t)
{
	counter_tick = t;
}

void tick_counter(void)
{
	counter_tick++;
    if (get_tick() >= 60000)
    {
        set_tick(0);
    }
}

void delay_ms(unsigned long ms)
{
	#if 1
    unsigned long tickstart = get_tick();
    unsigned long wait = ms;
	unsigned long tmp = 0;
	
    while (1)
    {
		if (get_tick() > tickstart)	// tickstart = 59000 , tick_counter = 60000
		{
			tmp = get_tick() - tickstart;
		}
		else // tickstart = 59000 , tick_counter = 2048
		{
			tmp = 60000 -  tickstart + get_tick();
		}		
		
		if (tmp > wait)
			break;
    }
	
	#else
	TIMER_Delay(TIMER0, 1000*ms);
	#endif
}

void TC9548A_DA7280_Ctrl(unsigned char idx_drv , unsigned char idx_waveform)
{                 
    unsigned long timeout = 0;

    TCA9548A_SetChannel(idx_drv);
    DA7280_PlaybackIndex(idx_waveform , 0 ,0);

    #if 0
    timeout = 0xFFFFFFFF;

    while (1)
    {
        if (read_DA7280_trig_nIRQ(idx_drv))
        {
            clr_DA7280_trig_nIRQ(idx_drv);
            DA7280_PlaybackFinishCheck();
            printf("DA7280 exec finish(%d)\r\n\r\n",idx_drv);
            break;
        }
        else
        {
            if (timeout-- == 0)
            {
                printf("DA7280 exec TIMEOUT(%d)\r\n\r\n",idx_drv);
                break;
            }
        }
    }    
    #else
    if (read_DA7280_trig_nIRQ(idx_drv))
    {
        clr_DA7280_trig_nIRQ(idx_drv);
        DA7280_PlaybackFinishCheck();
        printf("DA7280 exec finish(%d)\r\n\r\n",idx_drv);
    }
    #endif
}


void TC9548A_DA7280_Ctrl_All(unsigned char idx_drv , unsigned char idx_waveform)
{                       
    unsigned char i = 0;

    for( i = 1 ; i <=8 ; i++)
    {
        TC9548A_DA7280_Ctrl(i,idx_waveform);
    }
    
}

void btnX_Debounce(BTN_IDX_t idx,unsigned char IOState)
{
    // printf("%d,%2d\r\n",event_btn[idx].ButtonState,event_btn[idx].ButtonDebounceTimer);

    if ((event_btn[idx].ButtonState == IOState) && 
        (event_btn[idx].ButtonDebounceTimer++ == DEBOUNCE_INTERVAL_TIME))
    {
        event_btn[idx].ButtonDebounceTimer = 0;
        event_btn[idx].ButtonDebounceState |= 1 << event_btn[idx].ButtonBitShift;

        // printf("0x%02X,%d\r\n",event_btn[idx].ButtonDebounceState,event_btn[idx].ButtonBitShift);
        if (event_btn[idx].ButtonDebounceState == 0xFF)
        {
            event_btn[idx].ButtonShortPressed = PRESSED;
            event_btn[idx].ButtonDebounceState = 0;
            event_btn[idx].ButtonDebounceCnt++;
        }

        if ((event_btn[idx].ButtonShortPressed == PRESSED) && 
            (event_btn[idx].ButtonDebounceCnt >=  (DEBOUNCE_LONG_TIME/DEBOUNCE_SHORT_TIME)  ) )    // over 80ms* __ = long
        {
            event_btn[idx].ButtonLongPressed = PRESSED;
            event_btn[idx].ButtonDebounceCnt = 0;            
        }
        event_btn[idx].ButtonBitShift = (event_btn[idx].ButtonBitShift >= 7) ? (0) : (event_btn[idx].ButtonBitShift+1) ;
    }
}

void btnX_init(BTN_IDX_t idx)
{
    event_btn[idx].ButtonDebounceTimer = 0;
    event_btn[idx].ButtonDebounceState = 0;
    event_btn[idx].ButtonDebounceCnt = 0;
    event_btn[idx].ButtonShortPressed = RELEASED;
    event_btn[idx].ButtonLongPressed = RELEASED;
    event_btn[idx].ButtonBitShift = 0;
}

void btn8_timer_irq(void)
{
    if (BTN8 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_8,BTN8);
    }
    else
    {
        FLAG_BTN_8_SHORT_PRESSED = 0;
        FLAG_BTN_8_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_8);
    }
}

void btn8_task(void)
{

    /********************* */
    if ((FLAG_BTN_8_SHORT_PRESSED == 0) && event_btn[BTN_IDX_8].ButtonShortPressed)
    {
        FLAG_BTN_8_SHORT_PRESSED = 1;
        printf("button 8 SHORT pressed\r\n");
                                
        TC9548A_DA7280_Ctrl(8,SEQUENCE_IDX0);
    }

    if ((FLAG_BTN_8_LONG_PRESSED == 0 ) && event_btn[BTN_IDX_8].ButtonLongPressed)
    {
        FLAG_BTN_8_LONG_PRESSED = 1;
        printf("button 8 LONG pressed\r\n");
    }
}

void btn7_timer_irq(void)
{
    if (BTN7 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_7,BTN7);
    }
    else
    {
        FLAG_BTN_7_SHORT_PRESSED = 0;
        FLAG_BTN_7_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_7);
    }
}

void btn7_task(void)
{

    /********************* */
    if ((FLAG_BTN_7_SHORT_PRESSED == 0) && event_btn[BTN_IDX_7].ButtonShortPressed)
    {
        FLAG_BTN_7_SHORT_PRESSED = 1;
        printf("button 7 SHORT pressed\r\n");
                                
        TC9548A_DA7280_Ctrl(7,SEQUENCE_IDX0);
    }

    if ((FLAG_BTN_7_LONG_PRESSED == 0 ) && event_btn[BTN_IDX_7].ButtonLongPressed)
    {
        FLAG_BTN_7_LONG_PRESSED = 1;
        printf("button 7 LONG pressed\r\n");
    }
}

void btn6_timer_irq(void)
{
    if (BTN6 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_6,BTN6);
    }
    else
    {
        FLAG_BTN_6_SHORT_PRESSED = 0;
        FLAG_BTN_6_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_6);
    }
}

void btn6_task(void)
{

    /********************* */
    if ((FLAG_BTN_6_SHORT_PRESSED == 0) && event_btn[BTN_IDX_6].ButtonShortPressed)
    {
        FLAG_BTN_6_SHORT_PRESSED = 1;
        printf("button 6 SHORT pressed\r\n");
                                
        TC9548A_DA7280_Ctrl(6,SEQUENCE_IDX0);
    }

    if ((FLAG_BTN_6_LONG_PRESSED == 0 ) && event_btn[BTN_IDX_6].ButtonLongPressed)
    {
        FLAG_BTN_6_LONG_PRESSED = 1;
        printf("button 6 LONG pressed\r\n");
    }
}

void btn5_timer_irq(void)
{
    if (BTN5 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_5,BTN5);
    }
    else
    {
        FLAG_BTN_5_SHORT_PRESSED = 0;
        FLAG_BTN_5_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_5);
    }
}

void btn5_task(void)
{

    /********************* */
    if ((FLAG_BTN_5_SHORT_PRESSED == 0) && event_btn[BTN_IDX_5].ButtonShortPressed)
    {
        FLAG_BTN_5_SHORT_PRESSED = 1;
        printf("button 5 SHORT pressed\r\n");
                                
        TC9548A_DA7280_Ctrl(5,SEQUENCE_IDX0);
    }

    if ((FLAG_BTN_5_LONG_PRESSED == 0 ) && event_btn[BTN_IDX_5].ButtonLongPressed)
    {
        FLAG_BTN_5_LONG_PRESSED = 1;
        printf("button 5 LONG pressed\r\n");
    }
}

void btn4_timer_irq(void)
{
    if (BTN4 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_4,BTN4);
    }
    else
    {
        FLAG_BTN_4_SHORT_PRESSED = 0;
        FLAG_BTN_4_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_4);
    }
}

void btn4_task(void)
{

    /********************* */
    if ((FLAG_BTN_4_SHORT_PRESSED == 0) && event_btn[BTN_IDX_4].ButtonShortPressed)
    {
        FLAG_BTN_4_SHORT_PRESSED = 1;
        printf("button 4 SHORT pressed\r\n");
                                
        // TC9548A_DA7280_Ctrl(4,SEQUENCE_IDX0);
        TC9548A_DA7280_Ctrl_All(9,SEQUENCE_IDX3);   // all haptics , waveform 4
    }

    if ((FLAG_BTN_4_LONG_PRESSED == 0 ) && event_btn[BTN_IDX_4].ButtonLongPressed)
    {
        FLAG_BTN_4_LONG_PRESSED = 1;
        printf("button 4 LONG pressed\r\n");
    }
}

void btn3_timer_irq(void)
{
    if (BTN3 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_3,BTN3);
    }
    else
    {
        FLAG_BTN_3_SHORT_PRESSED = 0;
        FLAG_BTN_3_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_3);
    }
}

void btn3_task(void)
{

    /********************* */
    if ((FLAG_BTN_3_SHORT_PRESSED == 0) && event_btn[BTN_IDX_3].ButtonShortPressed)
    {
        FLAG_BTN_3_SHORT_PRESSED = 1;
        printf("button 3 SHORT pressed\r\n");
                          
        // TC9548A_DA7280_Ctrl(3,SEQUENCE_IDX0);      
        TC9548A_DA7280_Ctrl_All(9,SEQUENCE_IDX2);   // all haptics , waveform 3
    }

    if ((FLAG_BTN_3_LONG_PRESSED == 0 ) && event_btn[BTN_IDX_3].ButtonLongPressed)
    {
        FLAG_BTN_3_LONG_PRESSED = 1;
        printf("button 3 LONG pressed\r\n");
    }
}

void btn2_timer_irq(void)
{
    if (BTN2 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_2,BTN2);
    }
    else
    {
        FLAG_BTN_2_SHORT_PRESSED = 0;
        FLAG_BTN_2_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_2);
    }
}

void btn2_task(void)
{

    /********************* */
    if ((FLAG_BTN_2_SHORT_PRESSED == 0) && event_btn[BTN_IDX_2].ButtonShortPressed)
    {
        FLAG_BTN_2_SHORT_PRESSED = 1;
        printf("button 2 SHORT pressed\r\n");
                              
        // TC9548A_DA7280_Ctrl(2,SEQUENCE_IDX0);  
        TC9548A_DA7280_Ctrl_All(9,SEQUENCE_IDX1);   // all haptics , waveform 2
    }

    if ((FLAG_BTN_2_LONG_PRESSED == 0 ) && event_btn[BTN_IDX_2].ButtonLongPressed)
    {
        FLAG_BTN_2_LONG_PRESSED = 1;
        printf("button 2 LONG pressed\r\n");
    }
}

void btn1_timer_irq(void)
{
    if (BTN1 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_1,BTN1);
    }
    else
    {
        FLAG_BTN_1_SHORT_PRESSED = 0;
        FLAG_BTN_1_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_1);
    }
}

void btn1_task(void)
{

    /********************* */
    if ((FLAG_BTN_1_SHORT_PRESSED == 0) && event_btn[BTN_IDX_1].ButtonShortPressed)
    {
        FLAG_BTN_1_SHORT_PRESSED = 1;
        printf("button 1 SHORT pressed\r\n");
                          
        // TC9548A_DA7280_Ctrl(1,SEQUENCE_IDX0);      
        TC9548A_DA7280_Ctrl_All(9,SEQUENCE_IDX0);   // all haptics , waveform 1
    }

    if ((FLAG_BTN_1_LONG_PRESSED == 0 ) && event_btn[BTN_IDX_1].ButtonLongPressed)
    {
        FLAG_BTN_1_LONG_PRESSED = 1;
        printf("button 1 LONG pressed\r\n");
    }
}

/*
    BTN1:P90
    BTN2:P91
    BTN3:P92
    BTN4:P93
    BTN5:P94
    BTN6:P95
    BTN7:P96
    BTN8:P97
*/

void btn_timer_irq(void)
{
    btn1_timer_irq();
    btn2_timer_irq();
    btn3_timer_irq();
    btn4_timer_irq();
    btn5_timer_irq();
    btn6_timer_irq();
    btn7_timer_irq();
    btn8_timer_irq();
}

void btn_init(void)
{ 
    btnX_init(BTN_IDX_1);
    btnX_init(BTN_IDX_2);
    btnX_init(BTN_IDX_3);
    btnX_init(BTN_IDX_4);
    btnX_init(BTN_IDX_5);
    btnX_init(BTN_IDX_6);
    btnX_init(BTN_IDX_7);    
    btnX_init(BTN_IDX_8);
}

void btn_task(void)
{
    btn1_task();
    btn2_task();
    btn3_task();
    btn4_task();
    btn5_task();
    btn6_task();
    btn7_task();
    btn8_task();
}

/*
    SCL	    23 : P62/SCL0
    SDA	    24 : P63/SDA0
    GPIO	40 : P32/INTP7

    // TCA9548A-Q1 , DA7280-A
    SCL     38 : P71/SCL11
    SDA     39 : P70/SDA11


    SCL	    54 : P10/SCL10
    SDA	    53 : P11/SDA10
    GPIO	29 : P00/INTP9
*/

/*
    GPIO0:P13
    GPIO1:P46
    GPIO2:P45
*/

void Init_all_DA7280_GPIO(void)
{
    DA7280_GPI_0 = 0;
    DA7280_GPI_1 = 0;
    DA7280_GPI_2 = 0;
}


/*
    nIRQ1:P125/INTP1
    nIRQ2:P120/INTP4
    nIRQ3:P137/INTP0
    nIRQ4:P30/INTP2

    nIRQ5:P17/INTP3
    nIRQ6:P51/INTP11
    nIRQ7:P53/INTP10
    nIRQ8:P12/INTP5
*/

void Init_all_DA7280_nIRQ(void)
{
    R_INTC1_Start();
    R_INTC4_Start();
    R_INTC0_Start();
    R_INTC2_Start();

    R_INTC3_Start();
    R_INTC11_Start();
    R_INTC10_Start();
    R_INTC5_Start();
}

void Init_all_DA7280_cfg(void)
{
    unsigned char i = 1;

    TCA9548A_reset(1);

    for (i = 1 ; i <= 8 ; i++)
    {
        TCA9548A_SetChannel(i);
        printf("\r\nDA7280(%d)\r\n",i);
        DA7280_init();        
    }

    // TC9548A_DA7280_Ctrl_All(9,SEQUENCE_IDX0);
}

void Timer_1ms_IRQ(void)
{
    tick_counter();

    if ((get_tick() % 1000) == 0)
    {
        FLAG_PROJ_TIMER_PERIOD_1000MS = 1;
    }

    if ((get_tick() % 2000) == 0)
    {
        DA7280_set_TIMER_PERIOD();
    }	

    if ((get_tick() % 50) == 0)
    {

    }	

    btn_timer_irq();

}


/*
    F13 target board
    LED1 connected to P66, LED2 connected to P67
*/
// void LED_Toggle(void)
// {
//     // PIN_WRITE(6,6) = ~PIN_READ(6,6);
//     // PIN_WRITE(6,7) = ~PIN_READ(6,7);
//     P6_bit.no6 = ~P6_bit.no6;
//     P6_bit.no7 = ~P6_bit.no7;
// }

void loop(void)
{
	static unsigned long LOG1 = 0;
	static unsigned long specific_cnt = 0;

    if (FLAG_PROJ_TIMER_PERIOD_1000MS)
    {
        FLAG_PROJ_TIMER_PERIOD_1000MS = 0;

        if (++specific_cnt == 60)
        {
            printf("log(timer):%4d\r\n",LOG1++);
            specific_cnt = 0;
        }
        // LED_Toggle();             
    }

    // DA7280_Process(key_buffer);
    btn_task();
}

// F24 EVB , P137/INTP0 , set both edge 
// void Button_Process_long_counter(void)
// {
//     if (FLAG_PROJ_TRIG_BTN2)
//     {
//         btn_tick_counter();
//     }
//     else
//     {
//         btn_set_tick(0);
//     }
// }

// void Button_Process_in_polling(void)
// {
//     static unsigned char cnt = 0;

//     if (FLAG_PROJ_TRIG_BTN1)
//     {
//         FLAG_PROJ_TRIG_BTN1 = 0;
//         printf("BTN pressed(%d)\r\n",cnt);

//         if (cnt == 0)
//         {
//             FLAG_PROJ_TRIG_BTN2 = 1;
//         }
//         else if (cnt == 1)
//         {
//             FLAG_PROJ_TRIG_BTN2 = 0;
//         }

//         cnt = (cnt >= 1) ? (0) : (cnt+1) ;
//     }

//     if ((FLAG_PROJ_TRIG_BTN2 == 1) && 
//         (btn_get_tick() > BTN_PRESSED_LONG))
//     {         
//         printf("entry demo mode(%d)\r\n" , FLAG_PROJ_DEMO_MODE);
//         btn_set_tick(0);
//         /*
//             FLAG_PROJ_DEMO_MODE
//             0 : //first trig , enable demo mode
//             1 : //sec trig , disable demo mode
//         */
//         DA7280_demo_enable(FLAG_PROJ_DEMO_MODE);   

//         FLAG_PROJ_DEMO_MODE = ~FLAG_PROJ_DEMO_MODE;
//     }
// }

// F13 EVB , P137/INTP0
// void Button_Process_in_IRQ(void)    
// {
//     FLAG_PROJ_TRIG_BTN1 = 1;
// }

void UARTx_Process(unsigned char rxbuf)
{    
    if (rxbuf > 0x7F)
    {
        printf("invalid command\r\n");
    }
    else
    {
        printf("press:%c(0x%02X)\r\n" , rxbuf,rxbuf);   // %c :  C99 libraries.
        switch(rxbuf)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'a':
            case 'B':
            case 'b':
            case 'C':
            case 'c':
            case 'D':
            case 'd':
            case 'E':
            case 'e':
            case 'F':
            case 'f':
                key_buffer = rxbuf;
                flag_play = 1;
                break;



            case 'X':
            case 'x':
                RL78_soft_reset(7);
                break;
            case 'Z':
            case 'z':
                RL78_soft_reset(1);
                break;
        }
    }
}


/*
    7:Internal reset by execution of illegal instruction
    1:Internal reset by illegal-memory access
*/
//perform sofware reset
void _reset_by_illegal_instruction(void)
{
    static const unsigned char illegal_Instruction = 0xFF;
    void (*dummy) (void) = (void (*)(void))&illegal_Instruction;
    dummy();
}
void _reset_by_illegal_memory_access(void)
{
    #if 1
    const unsigned char ILLEGAL_ACCESS_ON = 0x80;
    IAWCTL |= ILLEGAL_ACCESS_ON;            // switch IAWEN on (default off)
    *(__far volatile char *)0x00000 = 0x00; //write illegal address 0x00000(RESET VECTOR)
    #else
    signed char __far* a;                   // Create a far-Pointer
    IAWCTL |= _80_CGC_ILLEGAL_ACCESS_ON;    // switch IAWEN on (default off)
    a = (signed char __far*) 0x0000;        // Point to 0x000000 (FLASH-ROM area)
    *a = 0;
    #endif
}

void RL78_soft_reset(unsigned char flag)
{
    switch(flag)
    {
        case 7: // do not use under debug mode
            _reset_by_illegal_instruction();        
            break;
        case 1:
            _reset_by_illegal_memory_access();
            break;
    }
}


// retarget printf
int __far putchar(int c)
{
    // F13 , UART0
    STMK0 = 1U;    /* disable INTST0 interrupt */
    SDR00L = (unsigned char)c;
    while(STIF0 == 0)
    {

    }
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    return c;
}

void hardware_init(void)
{
    // const unsigned char indicator[] = "hardware_init";
    EI();   //BSP_EI();
    R_UART0_Start();            // P15:TXD0, P16:RXD0
    R_TMR_RJ0_Start();  


    /*
        The system master can reset the TCA9548A in the
        event of a time-out or other improper operation by
        asserting a low in the RESET input      
    */
    TCA9548A_reset(0);

    btn_init();

    Init_all_DA7280_GPIO();
    Init_all_DA7280_nIRQ();
    // DA7280_init();           // P71:SCL11,P70:SDA11
    Init_all_DA7280_cfg();
    
    printf("\r\n%s finish\r\n",__func__);
}
