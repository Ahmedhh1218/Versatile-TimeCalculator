/******************************************************************************
 *
 * Module:  KEYPAD
 *
 * File Name: keypad.h
 *
 * Author: Team 3
 * 
 * Description: Driver to use the keypad from the HAL layer
 *
 *******************************************************************************/
#include "keypad.h"
extern uint16 mode;
extern uint16 counter;

// mapping of the keypad to match the required behavior from hardware
static const uint8 mapping[4][4] = { {'1', '2', '3', '+'},
                                     {'4', '5', '6', '-'},
                                     {'7', '8', '9', '/'},
                                     {'*', '0', '#', '='} };

void keypad_init(){
  SYSCTL_RCGCGPIO_R |= 0x14;                            //enable clc for port C & E
  while ((SYSCTL_RCGCGPIO_R&0x14)==0);                  //wait for clock to be enabled
  GPIO_PORTC_CR_R  |= 0xF0;                             //allow changes to all the bits in port C
  GPIO_PORTE_CR_R  |= 0x0F;                             //allow changes to all the bits in port E
  GPIO_PORTC_DIR_R |= 0xF0;                             //set directions cols are o/ps
  GPIO_PORTE_DIR_R |= 0x00;                             //set directions rows are i/ps
  GPIO_PORTE_PDR_R |= 0x0F;                             //pull down resistor on Rows
  GPIO_PORTC_DEN_R |= 0xF0;                             //digital enable pins in port C
  GPIO_PORTE_DEN_R |= 0x0F;                             //digital enable pins in port E
  
}
uint8 keypad_read(void)
{
  uint16 m1=mode;
  uint16 m2=counter;
  while(m1==mode && m2==counter)
  {
    uint8 i,j;
    for(i = 0; i < 4; i++)                              //columns traverse
    {
      GPIO_PORTC_DATA_R = (1<<(i+4));
      delayUs(2);
      for(j = 0; j < 4; j++)                            //rows traverse
      {
        if((GPIO_PORTE_DATA_R & 0x0F) & (1<<(j)))
        {
          delayMs(50);
          return mapping[j][i];
        }
      }
    }
  }
  return 't';
}