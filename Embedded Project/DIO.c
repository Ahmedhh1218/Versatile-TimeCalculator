/******************************************************************************
 *
 * Module:  Digital Input Output
 *
 * File Name: DIO.c
 *
 * Author: Team 3
 * 
 * Description: Driver used in MCAL layer to configure the pinouts
 *
 *******************************************************************************/
#include "types.h"
#include "tm4c123gh6pm.h"
#include "DIO.h"
#include "bitwise_operation.h"

void DIO_init(PORT_ID PORT,PORT_Configuration* Config_Ptr)
{
 SET_BIT(SYSCTL_RCGCGPIO_R,PORT);
 
 while((GET_BIT(SYSCTL_RCGCGPIO_R,PORT))==0){};

 switch(PORT) 
 {
   case PORTA:
     GPIO_PORTA_LOCK_R=Config_Ptr->lock; //unlock PORTA
  
  GPIO_PORTA_CR_R=Config_Ptr->CR; //Accept changes
  
  GPIO_PORTA_DIR_R=Config_Ptr->direction; // Define directions 
  
  GPIO_PORTA_PUR_R=Config_Ptr->pullup; //pullup
  
  GPIO_PORTA_DEN_R=Config_Ptr->enable;//enable pins
  
  break;
 
 case PORTB:
  GPIO_PORTB_LOCK_R=Config_Ptr->lock; //unlock PORTB
  
  GPIO_PORTB_CR_R=Config_Ptr->CR; //Accept changes
  
  GPIO_PORTB_DIR_R=Config_Ptr->direction; // Define directions 
  
  GPIO_PORTB_PUR_R=Config_Ptr->pullup; //pullup
  
  GPIO_PORTB_DEN_R=Config_Ptr->enable;//enable pins
  
   break;
   
  case PORTC:
  GPIO_PORTC_LOCK_R=Config_Ptr->lock; //unlock PORTC
  
  GPIO_PORTC_CR_R=Config_Ptr->CR; //Accept changes
  
  GPIO_PORTC_DIR_R=Config_Ptr->direction; // Define directions 
  
  GPIO_PORTC_PUR_R=Config_Ptr->pullup; //pullup
  
  GPIO_PORTC_DEN_R=Config_Ptr->enable;//enable pins
  
   break;
  
  case PORTD:
  GPIO_PORTD_LOCK_R=Config_Ptr->lock; //unlock PORTD
  
  GPIO_PORTD_CR_R=Config_Ptr->CR; //Accept changes
  
  GPIO_PORTD_DIR_R=Config_Ptr->direction; // Define directions 
  
  GPIO_PORTD_PUR_R=Config_Ptr->pullup; //pullup
  
  GPIO_PORTD_DEN_R=Config_Ptr->enable;//enable pins
 
  
   break;
   
  case PORTE:
  GPIO_PORTE_LOCK_R=Config_Ptr->lock; //unlock PORTE
  
  GPIO_PORTE_CR_R=Config_Ptr->CR; //Accept changes
  
  GPIO_PORTE_DIR_R=Config_Ptr->direction; // Define directions 
  
  GPIO_PORTE_PUR_R=Config_Ptr->pullup; //pullup
  
  GPIO_PORTE_DEN_R=Config_Ptr->enable;//enable pins
  
   break;
  
  case PORTF:
  GPIO_PORTF_LOCK_R=Config_Ptr->lock; //unlock PORTF
  
  GPIO_PORTF_CR_R=Config_Ptr->CR; //Accept changes
  
  GPIO_PORTF_DIR_R=Config_Ptr->direction; // Define directions 
  
  GPIO_PORTF_PUR_R=Config_Ptr->pullup; //pullup
  
  GPIO_PORTF_DEN_R=Config_Ptr->enable;//enable pins
  
   break;
   
   default:
   break;
 }

 
}
 
 void DIO_writePort(PORT_ID PORT,uint8 value)
 {
   switch(PORT)
   {
   case PORTA:
     GPIO_PORTA_DATA_R=value;
     break;
   case PORTB:
      GPIO_PORTB_DATA_R=value;
      break;
   case PORTC:
      GPIO_PORTC_DATA_R=value;
      break;
   case PORTD:
       GPIO_PORTD_DATA_R=value;
       break;
   case PORTE:
       GPIO_PORTE_DATA_R=value;
       break;
   case PORTF:
       GPIO_PORTF_DATA_R=value;
       break;
   default:
     break;
   }
 }
 
void DIO_writePin(PORT_ID PORT,PIN_ID PIN,uint8 value)
{
   switch(PORT)
   {
   case PORTA:
     if(value==LOGIC_LOW)
     {
       CLEAR_BIT(GPIO_PORTA_DATA_R,PIN);
     }
     else
     {
       SET_BIT(GPIO_PORTA_DATA_R,PIN);
     }
     break;
     case PORTB:
     if(value==LOGIC_LOW)
     {
       CLEAR_BIT(GPIO_PORTB_DATA_R,PIN);
     }
     else
     {
       SET_BIT(GPIO_PORTB_DATA_R,PIN);
     }
     break;
       case PORTC:
     if(value==LOGIC_LOW)
     {
       CLEAR_BIT(GPIO_PORTC_DATA_R,PIN);
     }
     else
     {
       SET_BIT(GPIO_PORTC_DATA_R,PIN);
     }
     break;
       case PORTD:
     if(value==LOGIC_LOW)
     {
       CLEAR_BIT(GPIO_PORTD_DATA_R,PIN);
     }
     else
     {
       SET_BIT(GPIO_PORTD_DATA_R,PIN);
     }
     break;
       case PORTE:
     if(value==LOGIC_LOW)
     {
       CLEAR_BIT(GPIO_PORTE_DATA_R,PIN);
     }
     else
     {
       SET_BIT(GPIO_PORTE_DATA_R,PIN);
     }
     break;
       case PORTF:
       
     if(value==LOGIC_LOW)
     {
       CLEAR_BIT(GPIO_PORTF_DATA_R,PIN);
     }
     else
     {
       SET_BIT(GPIO_PORTF_DATA_R,PIN);
     }
     break;
   default:
     break;
   }
}

uint8 DIO_readPort(PORT_ID PORT)
{
   switch(PORT)
   {
   case PORTA:
     return GPIO_PORTA_DATA_R;
     break;
   case PORTB:
        return GPIO_PORTB_DATA_R;
      break;
   case PORTC:
       return GPIO_PORTC_DATA_R;
      break;
   case PORTD:
        return GPIO_PORTD_DATA_R;
       break;
   case PORTE:
        return GPIO_PORTE_DATA_R;
       break;
   case PORTF:
         return GPIO_PORTF_DATA_R;
       break;
   default:
     return 0;
   }
  
}

uint8 DIO_readPin(PORT_ID PORT,PIN_ID PIN)
{
   switch(PORT)
   {
   case PORTA:
     return GET_BIT(GPIO_PORTA_DATA_R,PIN);
     break;
   case PORTB:
     return GET_BIT(GPIO_PORTB_DATA_R,PIN);
      break;
   case PORTC:
     return GET_BIT(GPIO_PORTC_DATA_R,PIN);
      break;
   case PORTD:
        return GET_BIT(GPIO_PORTD_DATA_R,PIN);
       break;
   case PORTE:
        return GET_BIT(GPIO_PORTE_DATA_R,PIN);
       break;
   case PORTF:
        return GET_BIT(GPIO_PORTF_DATA_R,PIN);
       break;
   default:
     return 0;
   }
}  