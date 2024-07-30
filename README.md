# RL78_F13_IIC11_I2C_TCA9548A_DA7280
 RL78_F13_IIC11_I2C_TCA9548A_DA7280

udpate @ 2024/07/30

1. initial below pin define to function 

- UART : P15:TXD0, P16:RXD0

- IIC11 : P71:SCL11,P70:SDA11

- DA7280*8 , with nIRQ control GPIO : 

	- nIRQ1:INTP1 , nIRQ2:INTP4 , nIRQ3:INTP0 , nIRQ4:INTP2

	- nIRQ5:INTP3 , nIRQ6:INTP11 , nIRQ7:INTP10 , nIRQ8:INTP5

- DA7280 , GPIO0:P13 , GPIO1:P46 , GPIO2:P45  

- TCA9548A RESET PIN : P44

- BUTTON : BTN1:P90 , BTN2:P91 , BTN3:P92 , BTN4:P93 , BTN5:P94 , BTN6:P95 , BTN7:P96 , BTN8:P97

2. by using I2C switch (TCA9548A)

send 0x01 , 0x02 , 0x04 , ... ,  to switch to different channel , check TCA9548A_SetChannel in TC9548A_DA7280_Ctrl

below is change to CH1 (0x01)
![image](https://github.com/released/RL78_F13_IIC11_I2C_TCA9548A_DA7280/blob/main/TCA9548A_SET_CH_1.jpg)

below is change to CH2 (0x02)
![image](https://github.com/released/RL78_F13_IIC11_I2C_TCA9548A_DA7280/blob/main/TCA9548A_SET_CH_2.jpg)

below is change to CH8 (0x80)
![image](https://github.com/released/RL78_F13_IIC11_I2C_TCA9548A_DA7280/blob/main/TCA9548A_SET_CH_8.jpg)

3. button behavior : 

- press button 1 will play waveform 1 , with DA7280*8

- press button 2 will play waveform 2 , with DA7280*8

- press button 3 will play waveform 3 , with DA7280*8

- press button 4 will play waveform 4 , with DA7280*8

![image](https://github.com/released/RL78_F13_IIC11_I2C_TCA9548A_DA7280/blob/main/TCA9548A_DA7280_3.jpg)


- press button 5 , will play waveform 1 at DA7280 no. 5

- press button 6 , will play waveform 1 at DA7280 no. 6

- press button 7 , will play waveform 1 at DA7280 no. 7

- press button 8 , will play waveform 1 at DA7280 no. 8

![image](https://github.com/released/RL78_F13_IIC11_I2C_TCA9548A_DA7280/blob/main/TCA9548A_DA7280_4.jpg)

4. 

below is LA capture  , switch to CH6 (0x20) , and play the waveform 1
![image](https://github.com/released/RL78_F13_IIC11_I2C_TCA9548A_DA7280/blob/main/TCA9548A_DA7280_2.jpg)


below is LA capture  , switch to CH1 (0x01) , and play the waveform 1
![image](https://github.com/released/RL78_F13_IIC11_I2C_TCA9548A_DA7280/blob/main/TCA9548A_DA7280_2.jpg)

