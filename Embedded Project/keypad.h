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

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "DIO.h"
#include "types.h"
#include "tm4c123gh6pm.h"
#include "LCD.h"
#define KEYPAD_NUM_COLS                   4
#define KEYPAD_NUM_ROWS                   4

void keypad_init(void);
/*
  Initialize the keypad 
*/
uint8 keypad_read(void);
/*
  Function that detects the pressed button by setting all rows to HIGH
  then checking if any column matches HIGH with rows
  then the intersection is the button pressed 
*/


#endif