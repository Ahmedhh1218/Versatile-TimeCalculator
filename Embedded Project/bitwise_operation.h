/******************************************************************************
 *
 * Module: BITWISE - OPERATIONS
 *
 * File Name: bitwise_operation.h
 *
 * Author: Team 3
 * 
 * Description: Commonly used Macros
 *
 *******************************************************************************/

#ifndef BITWISE
#define BITWISE

/* Set a certain bit in any register */
#define SET_BIT(REG,BIT) (REG |= (1<<BIT) )

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT) (REG&=~(1<<BIT))

/* Get the value of a certain bit*/
#define GET_BIT(REG,BIT) ((REG & (1<<BIT))>>BIT)

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

#endif