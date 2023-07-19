/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.h
 *
 * Author: Team 3 Referenced from Mazidi Textbook
 * 
 * Description: header file of the LCD driver
 *
 *******************************************************************************/

#include "types.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdlib.h> 

#define RS 0x20                         // PORTA BIT5 mask (Reset)
#define RW 0x40                         // PORTA BIT6 mask (Read/Write)
#define EN 0x80                         // PORTA BIT7 mask (Enable)

void delayMs(uint32 n);                 
/* 
  Delay in Milliseconds
  Parameter: n milliseconds
*/
void delayUs(uint32 n);                 
/* 
  Delay in Microseconds
  Parameter: n microseconds
*/
void LCD_command (uint8 command) ;      
/* 
  To Trigger certain operations in the LCD
  commands: 1  -->  Clear screen
          0x80 -->  sets the cursor at first col first row and increment by itself
          0xC0 -->  sets the cursor at first col second row and increment by itself
*/
void LCD_data(uint8 data);              
/* 
  Displays a character on the LCD
  parameter: The ASCII code of the character to be printed
*/
void LCD_printInt(uint32 no); 
/*
  Displays an Ingteger on the LCD 
  parameter: The integer to be printed itself
*/
void LCD_printString(char* str);
/*
  Displays a String on the LCD
  parameter: The string to be printed
*/
void LCD_init (void);
/*
  initializes the LCD
*/