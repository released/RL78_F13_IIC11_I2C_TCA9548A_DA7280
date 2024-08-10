/*_____ I N C L U D E S ____________________________________________________*/
// #include    <stdbool.h>

/*_____ D E C L A R A T I O N S ____________________________________________*/
#define bool  _Bool
/*_____ D E F I N I T I O N S ______________________________________________*/


#ifndef HIGH
#define HIGH              					            (1)
#endif

#ifndef LOW
#define LOW              					            (0)
#endif

#ifndef PRESSED
#define PRESSED              					        (1)
#endif

#ifndef RELEASED
#define RELEASED              					        (0)
#endif

//I2C switch reset pin : P44
#define TCA9548A_RESET                                  (P4_bit.no4)

/*
    GPIO0		51 : P13/TO04
    GPIO1		3 : P46
    GPIO2		4 : P45
*/

#define DA7280_GPI_0                                    (P1_bit.no3)
#define DA7280_GPI_1                                    (P4_bit.no6)
#define DA7280_GPI_2                                    (P4_bit.no5)

// button , schematic : pull high
#define BTN1                                            (P9_bit.no0)
#define BTN2                                            (P9_bit.no1)
#define BTN3                                            (P9_bit.no2)
#define BTN4                                            (P9_bit.no3)
#define BTN5                                            (P9_bit.no4)
#define BTN6                                            (P9_bit.no5)
#define BTN7                                            (P9_bit.no6)
#define BTN8                                            (P9_bit.no7)


/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/
extern unsigned char flag_play;

void set_TIMER_PERIOD_1000MS(void);
void reset_TIMER_PERIOD_1000MS(void);
bool Is_TIMER_PERIOD_1000MS_Trig(void);
void Timer_1ms_IRQ(void);

void delay_ms(unsigned long ms);

MD_STATUS drv_IIC11_I2C_read(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num);
MD_STATUS drv_IIC11_I2C_write(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num);

bool drv_Is_IIC11_bus_busy(void);
void drv_IIC11_callback_error(unsigned char err);
void drv_set_IIC11_receive_flag(bool flag);
bool drv_get_IIC11_receive_flag(void);
void drv_set_IIC11_send_flag(bool flag);
bool drv_get_IIC11_send_flag(void);

MD_STATUS DA7280_PlaybackFinishCheck(void);
void DA7280_forceStop(void);
void DA7280_playback_finish_check(void);
void DA7280_playback_idx(unsigned char idx , unsigned char loop , unsigned char en);
void DA7280_init(void);
void DA7280_key_filter(unsigned char key);
void DA7280_playback(void);
void DA7280_Process(unsigned char key);
void DA7280_nIRQ_Process_in_IRQ(void);

void clr_DA7280_trig_nIRQ(unsigned char nIRQ);
unsigned char read_DA7280_trig_nIRQ(unsigned char nIRQ);
void DA7280_trig_nIRQ(unsigned char nIRQ);

void DA7280_set_Init_Flag(bool flag);
bool DA7280_is_Init_Ready(void);

void DA7280_set_TIMER_PERIOD(void);
void DA7280_reset_TIMER_PERIOD(void);
bool DA7280_is_TIMER_PERIOD_Trig(void);
bool DA7280_is_demo_mode_enable(void);
void DA7280_demo_enable(unsigned char on);
void DA7280_demo_mode(void);

void UARTx_Process(unsigned char rxbuf);

void LED_Toggle(void);
void loop(void);

void RL78_soft_reset(unsigned char flag);
void hardware_init(void);
