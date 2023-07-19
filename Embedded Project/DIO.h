/******************************************************************************
 *
 * Module:  Digital Input Output
 *
 * File Name: DIO.h
 *
 * Author: Team 3
 * 
 * Description: Driver used in MCAL layer to configure the pinouts
 *
 *******************************************************************************/

#ifndef DIO
#define DIO
#include "types.h"

#define LOGIC_LOW 0
#define LOGIC_HIGH 1
typedef enum
{
    PORTA,PORTB,PORTC,PORTD,PORTE,PORTF
}PORT_ID;

typedef enum
{
  PIN0,PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7
}PIN_ID;

typedef enum {
  LOCKED,UNLOCKED=0X4C4F434B
}PORT_Lock;

typedef struct{
  PORT_Lock lock;
  uint8 CR;
  uint8 direction;
  uint8 pullup;
  uint8 enable;
   
}PORT_Configuration;

void DIO_init(PORT_ID PORT,PORT_Configuration* Config_Ptr);
void DIO_writePort(PORT_ID PORT,uint8 value);
void DIO_writePin(PORT_ID PORT,PIN_ID PIN,uint8 value);
uint8 DIO_readPort(PORT_ID PORT);
uint8 DIO_readPin(PORT_ID PORT,PIN_ID PIN);



#endif