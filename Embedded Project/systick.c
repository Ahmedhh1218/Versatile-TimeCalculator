/******************************************************************************
 *
 * Module: SYSTICK - TIMER
 *
 * File Name: systick.c
 *
 * Author: Team 3
 * 
 * Description: Systick timer module 
 *
 *******************************************************************************/

#include "types.h"
#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "systick.h"

void SysTick_Disable(void){
  CLEAR_BIT(NVIC_ST_CTRL_R,2);          
  CLEAR_BIT(NVIC_ST_CTRL_R,1);
  CLEAR_BIT(NVIC_ST_CTRL_R,0);
  NVIC_ST_RELOAD_R = 0;
  NVIC_ST_CURRENT_R = 0;
  if(GET_BIT(NVIC_ST_CTRL_R,16)){}
}

void SysTick_Enable(void){
  SET_BIT(NVIC_ST_CTRL_R,2);
  SET_BIT(NVIC_ST_CTRL_R,0);
}

uint32 SysTick_Period_Get(void){
  return NVIC_ST_CURRENT_R / 16000;
}

void SysTick_Period_Set(uint32 time_ms){
  NVIC_ST_RELOAD_R = time_ms*16000;
}

uint32 SysTick_Value_Get(void){
  return NVIC_ST_CURRENT_R;
}

uint8 SysTick_Is_Time_Out(void){
  if(GET_BIT(NVIC_ST_CTRL_R,16)){
    return 1;
  }
  else{
    return 0;
  }
}

void SysTick_Delay (uint32 n)
{
  CLEAR_BIT(NVIC_ST_CTRL_R,2);
  CLEAR_BIT(NVIC_ST_CTRL_R,1);
  CLEAR_BIT(NVIC_ST_CTRL_R,0);
  NVIC_ST_RELOAD_R = n-1;
  NVIC_ST_CURRENT_R = 0;
  SET_BIT(NVIC_ST_CTRL_R,2);
  SET_BIT(NVIC_ST_CTRL_R,0);
  while((NVIC_ST_CTRL_R & (1<<16))==0){}

}
void SysTick_1ms (uint32 delay)
{
  for(int i=0;i<delay;i++){
    SysTick_Delay(16000);
    
  }
}