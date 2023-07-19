/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.c
 *
 * Author: Team 3 Referenced from Mazidi Textbook
 * 
 * Description: Source file of the LCD driver
 *
 *******************************************************************************/

#include "LCD.h"

void LCD_init (void)
{
  
  SYSCTL_RCGCGPIO_R |= 0x01;
  
  SYSCTL_RCGCGPIO_R |=0x02;
  GPIO_PORTA_DIR_R |= 0xE0;
  GPIO_PORTA_DEN_R |= 0xE0;
  GPIO_PORTB_DIR_R  = 0xFF;
  GPIO_PORTB_DEN_R  = 0xFF;
  
  delayMs (100);
  LCD_command (0x30) ;
  delayMs (150);
  LCD_command (0x30) ;
  delayUs (100);
  LCD_command (0x30);
  LCD_command (0x38) ; 
  LCD_command (0x06) ;
  LCD_command (0x01) ; 
  LCD_command (0x0F) ; 
  
}

void LCD_command (uint8 command)
{
  GPIO_PORTA_DATA_R=0;
  GPIO_PORTB_DATA_R=command;
  GPIO_PORTA_DATA_R=EN;
  delayMs(10);
  GPIO_PORTA_DATA_R=0;
  if (command<4)
  {
    delayMs(5);
  }
  else
  {
    delayMs(5);
  }
}

void LCD_data(uint8 data)
{
  GPIO_PORTA_DATA_R=RS;
  GPIO_PORTB_DATA_R=data;
  GPIO_PORTA_DATA_R=(EN | RS) ;
  delayMs(10);
  GPIO_PORTA_DATA_R=0;
  delayMs(10);
}

void delayMs(uint32 n)
{
  
  int i,j;
  for(i=0;i<n;i++)
  {
    for(j=0;j<3180;j++)
    {}
  }
}
void delayUs(uint32 n)
{
  int i,j;
  for(i=0;i<n;i++)
  {
    for(j=0;j<3;j++)
    {}
  }
}

void LCD_printInt(uint32 no)
{
  char toprint[4] = {0};
  sprintf(toprint, "%lu", no);
  int i = 0;
  while(toprint[i] != '\0')
  {
    LCD_data(toprint[i]);
    i++;
  }
}


void LCD_printString(char* str)
{
  LCD_command(0x0C);
  int i = 0;
  while (str[i] != '\0')
  {
    LCD_data(str[i]);
    i++;
  }
}
