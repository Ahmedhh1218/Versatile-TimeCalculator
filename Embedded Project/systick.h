/******************************************************************************
 *
 * Module: SYSTICK - TIMER
 *
 * File Name: systick.h
 *
 * Author: Team 3
 * 
 * Description: Systick timer module 
 *
 *******************************************************************************/

#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "types.h"

void SysTick_Disable(void);
/*
  Function that disables the systick timer
*/
void SysTick_Enable(void);
/*
  Function that enables the systick timer
*/
uint32 SysTick_Period_Get(void);
void SysTick_Period_Set(uint32);
/*
  used to set the period to be counted
*/
uint32 SysTick_Value_Get(void);
/*
  Function that gets the current period
*/
uint8 SysTick_Is_Time_Out(void);
/*
  checks if time set has passed
*/
void SysTick_Delay(uint32 n);
/*
  Function used to count n milliseconds
*/
void SysTick_1ms (uint32 delay);

#endif
