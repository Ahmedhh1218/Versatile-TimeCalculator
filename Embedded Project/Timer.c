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

#include "Timer.h"

void timer_init(void)
{
  
 SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);          // clock enable on timer 0
 while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))); // wait until clock is enable
 TimerDisable (TIMER0_BASE,TIMER_BOTH);                 // disable timer
 TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);       // choose timer working configuration
 TimerLoadSet(TIMER0_BASE, TIMER_A, 16000000-1);        // choose Timer Reg A and set reload value 
 
}

void timer_enable(void)
{
 TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);        // enable the timer interrupts
 TimerEnable(TIMER0_BASE,TIMER_BOTH);                   // enable the timer itself using both register A & B,knowing that the reload value is set on Reg A
 IntEnable(INT_TIMER0A);                                
 IntMasterEnable();                                     // enable all interrupts
}

