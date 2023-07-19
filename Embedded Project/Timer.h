/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.h
 *
 * Author: Team 3
 * 
 * Description: General Purpose Timer Module
 *
 *******************************************************************************/

#ifndef TIMER_H
#define TIMER_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

void timer_init(void);
/*
  Function that initializes the timer module to be ready for starting
*/
void timer_enable(void);
/*
  fires the timer
*/
#endif