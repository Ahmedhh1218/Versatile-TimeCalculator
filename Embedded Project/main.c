/******************************************************************************************************************
 *
 * Module:  Application
 *
 * File Name: main.c
 *
 * Author: Team 3
 * 
 * Description: An application that have 3 modes of operation (Calculator, Stopwatch, Timer)
 *              with allowance of Switching modes at any moment.
 *              *******************************   STOPWATCH   *****************************
 *              the Stopwatch mode starts automatically and is supported with 3 pushbuttons 
 *              - 1 for pausing the stopwatch
 *              - 1 for resuming the stopwatch
 *              - 1 for resetting the stopwatch
 *              *******************************     TIMER    ******************************
 *              the timer mode once open waits for capturing input from the keypad and then pressing '='
 *              supports up till 59:59 counting down
 *              once counting is done, the red led in the hardware will light up
 *              *******************************  CALCULATOR  ******************************
 *              calculator mode supports 4 mathematical operations (Adding, Subtracting, Dividing, Multiplication)
 *              takes the first number from the user and waits for the operator to be pressed
 *              then the second number is taken and the result is printed and keeps available for more calculations
 *
 ******************************************************************************************************************/

#include "keypad.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "Timer.h"
#include "ctype.h"


/*************************************************************************************
*
*                               VARIABLE DECLARATION
*
*************************************************************************************/

uint8 sec1 = '0';                       // Timer        x  x : x S1
uint8 sec2 = '0';                       // Timer        x  x : S2 x
uint8 min1 = '0';                       // Timer        x  M1: x  x
uint8 min2 = '0';                       // Timer        M2 x : x  x
uint8 sw_sec1 = '0';                    // StopWatch    x  x : x S1
uint8 sw_sec2 = '0';                    // StopWatch    x  x : S2 x
uint8 sw_min1 = '0';                    // StopWatch    x  M1: x  x
uint8 sw_min2 = '0';                    // StopWatch    M2 x : x  x
uint16 counter = 0;
float32 remainderz = 0;
uint16 mode = 0;                        // counter for knowing the current mode


/*************************************************************************************
*
*                               FUNCTIONS PROTOTYPES
*
*************************************************************************************/
void modes(void);
/*
  mode control button connected on pin 4 PORTF
  if mode variable reaches 4 it reset again to 1 to keep on looping between modes
*/
void stopwatch_count(void);
/*
  function for incrementing the stopwatch
*/
void timertake(void);
/*
  function that allows input of values to initialize with the timer
*/
void timerinterrupt(void);
/*
  function that Decrement the timer
*/
void stopwatch_display(void);
/*
  function for displaying the counting of the stopwatch
*/
void interrupt(void);
/*
  Initializing All GPIO Interrupts and Buttons for Stopwatch
*/
void display(void);
/*
  Calculator Mode
*/

/*************************************************************************************
*
*                               MAIN APPLICATION
*
*************************************************************************************/
int main(void)
{

  keypad_init();                                                // initialize the keypad
  LCD_init();                                                   // initialize the LCD
  timer_init();                                                 // initialize the timer
  SYSCTL_RCGCGPIO_R |= 0x20;                                    // enable the clock to PORTF
  GPIO_PORTF_DIR_R |= (1 << 1);                                 // pin 1 set as output for the LED
  GPIO_PORTF_DEN_R |= (1 << 1);                                 // pin 1 Digital enable
  GPIO_PORTF_DEN_R |= (1 << 4);                                 // pin 4 Digital enable
  GPIO_PORTF_PDR_R |= (1 << 4);                                 // pin 4 set as pull down

  LCD_command(1);
  LCD_command(0x80);
  SysTickDisable();
  SysTickIntDisable();
  SysTickPeriodSet(16000000 - 1);                               // 1 second period set
  SysTickEnable();
  LCD_printString("Welcome");
  LCD_command(0xC0);
  LCD_printString("TEAM 3 PROJECT");
  delayMs(2000);

  GPIOIntRegister(GPIO_PORTF_BASE, modes);                      // when an interrupt generates it calls the modes function
  GPIOIntTypeSet(GPIO_PORTF_BASE, 0x10, GPIO_FALLING_EDGE);     // detect falling edges on pin 4
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);               // enable interrupts on pin 4

 
  while (1)
  {
    switch (mode)
    {

    case 1:
      timertake();                                              // Calling timer function
      break;
    case 2:
      SysTickDisable();                                         // DISABLE Timer 
      // reset values
      sec1 = '0';
      sec2 = '0';
      min1 = '0';
      min2 = '0';
      sw_sec1 = '0';
      sw_sec2 = '0';
      sw_min1 = '0';
      sw_min2 = '0';

      stopwatch_display();                                      // Calling Stopwatch display to keep showing stopwatch format
      if (keypad_read() == '=')
      {
        // IF = IS PRESSED START THE STOPWATCH
        IntRegister(INT_TIMER0A, stopwatch_count);
        interrupt();                                            // stopwatch operations interrupts setup
        timer_enable();                                                                                
      }

      while (mode == 2) {} // STAY IN THIS CASE IF MODE DOESNT CHANGE

      break;
    case 3:
      TimerDisable(TIMER0_BASE, TIMER_BOTH); // FOR STOPWATCH
      sw_sec1 = '0';
      sw_sec2 = '0';
      sw_min1 = '0';
      sw_min2 = '0';

      display(); // CALLING CALCULATOR

      break;
    default:
      break;
    }
  }
}


/*************************************************************************************
*
*                               FUNCTIONS DEFINITION
*
*************************************************************************************/

// FUNCTION FOR THE STOPWATCH INTERRUPTS
void pause()
{
  // IF INTERRUPT CAME FROM PIN 2, DISABLE TIMER (PAUSE STOPWATCH)
  // CLEAR INTERRUPT FLAG
  if (GPIOIntStatus(GPIO_PORTA_BASE, 0x04) == 0x04)
  {
    TimerDisable(TIMER0_BASE, TIMER_BOTH);
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
  }
  // IF INTERRUPT CAME FROM PIN 3, ENABLE TIMER (RESUME STOPWATCH)
  // CLEAR INTERRUPT FLAG
  else if (GPIOIntStatus(GPIO_PORTA_BASE, 0x08) == 0x08)
  {

    timer_enable();
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_3);
  }
   // IF INTERRUPT CAME FROM PIN 4, RESET Values
   // CLEAR INTERRUPT FLAG
  else if (GPIOIntStatus(GPIO_PORTA_BASE, 0x10) == 0x10)
  {
    sw_sec1 = '0';
    sw_sec2 = '0';
    sw_min1 = '0';
    sw_min2 = '0';

    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_4);
  }
}

/*****************************************************************************************
******************************************************************************************/

// INITIALIZING ALL GPIO INTERRUPTS AND BUTTONS FOR STOPWATCH OPERATIONS
void interrupt(void)
{
  GPIO_PORTA_DEN_R |= (1 << 2);                                 // assigned for the pause button
  GPIO_PORTA_DEN_R |= (1 << 3);                                 // assigned for the resume button
  GPIO_PORTA_DEN_R |= (1 << 4);                                 // assigned for the reset button
  GPIO_PORTA_PDR_R |= (1 << 2);
  GPIO_PORTA_PDR_R |= (1 << 3);
  GPIO_PORTA_PDR_R |= (1 << 4);
 
  GPIOIntRegister(GPIO_PORTA_BASE, pause);
  GPIOIntTypeSet(GPIO_PORTA_BASE, 0x04, GPIO_FALLING_EDGE);
  GPIOIntTypeSet(GPIO_PORTA_BASE, 0x08, GPIO_FALLING_EDGE);
  GPIOIntTypeSet(GPIO_PORTA_BASE, 0x10, GPIO_FALLING_EDGE);

  GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
  GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_3);
  GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_4);

}

/************************************************************************************************
*************************************************************************************************/

// FUNCTION THAT TAKES THE NUMBERS AND OPERATION FROM CALCULATOR
// RETURNS THE RESULT
uint32 calculator(float32 num1, float32 num2, uint8 op)
{

  uint32 answer;

  switch (op)
  {
  case '+':
    answer = ((uint32)num1 + (uint32)num2);
    break;
  case '-':
    answer = ((uint32)num1 - (uint32)num2);
    break;
  case '*':
    answer = ((uint32)num1 * (uint32)num2);
    break;
  case '/':
    answer = ((uint32)num1 / (uint32)num2);
    remainderz = (((float32)(num1 / num2) * 100) - ((uint32)answer * 100));
    break;
  default:
    break;
  }
  return answer;
}

/***********************************************************************************
************************************************************************************/

// CALCULATOR FUNCTION 
void display(void)
{
  LCD_command(1);                                               // CLEAR SCREEN
  LCD_command(0x80);                                            // MOVE CURSOR TO FIRST POSITION ON LCD
  LCD_printString("Calc Mode");                                 // PRINT CALC MODE ON LCD
  LCD_command(0xC0);                                            // MOVE CURSOR TO SECOND ROW
  float32 number1 = 0;
  float32 number2 = 0;
  uint8 op = 0;
  float32 result;
  uint8 x;
  int calcCounter = 0;
  int le = 0;
  while (mode == 3)
  {
    uint8 key = keypad_read(); 
    switch (calcCounter)
    {
    case 0:
      if (isdigit(key))
      {
        x = key;                                                // TAKE FIRST VALUE
        number1 = (number1 * 10) + (key - 48);                  // SAVE NUMBER AND MINUS 48 TO CHANGE FROM ASCII
        LCD_data(x);                                            // PRINT FIRST VALUE
        le++;                                                   // VARIABLE USED TO TAKE MORE NUMBERS
        delayMs(200);
      }
      else if (((key == '+') || (key == '-') || (key == '*') || (key == '/')) && le > 0)
      {
        // IF le IS MORE THAN 0 KEEP TAKING NUMBERS UNTIL YOU GET A SYMBOL
        op = key;                                               // SAVE OP TO SEND TO CALCULATOR FUNCTION
        LCD_data(op);                                           // PRINT THE OPERATION ON CALCULATOR
        delayMs(500);
        calcCounter++;                                          // WHEN TAKING OPERATION MOVE TO NEXT CASE
        le = 0; 
        delayMs(200);
      }
      break;
      // REPEAT FOR SECOND NUMBER
    case 1:
      if (isdigit(key))
      {
        x = key; 
        number2 = (number2 * 10) + (key - 48);
        LCD_data(x);
        le++;
        delayMs(200);
      }
      else if ((key == '=') && le > 0)
      {
        // WHEN KEYPAD GIVE = SEND RESULT TO CALCULATOR FUNCTION
        LCD_data('=');
        result = calculator(number1, number2, op);
        if (op == '/')
        {
          if (number2 == 0)
          {
            // IF NUMBER IS DIVIDED BY 0 PRINT MATH ERROR
            LCD_command(1);
            LCD_printString("MATH ERROR!");
            delayMs(100);
          }
          else
          {
            // IF OPERATION IS DIVISION PRINT IT AS FLOAT
            LCD_printInt((uint32)result);
            delayMs(1);
            LCD_data('.');
            delayMs(1);
            LCD_printInt((uint32)remainderz);
            delayMs(1000);
          }
        }
        else
        {
          // ELSE PRINT THE RESULT
          LCD_printInt((uint32)result);

          delayMs(250);
        }
        delayMs(1000); // WAIT FOR A SECOND
        LCD_command(1); // CLEAR SCREEN
        return;
      }
    }
  }
}

/**********************************************************************************
***********************************************************************************/

void stopwatch_display(void)
{
  LCD_command(1);
  LCD_command(0x80);
  LCD_printString("Stopwatch Mode");
  LCD_command(0xC0);
  LCD_printString("00:00");
}
void stopwatch_count(void)
{
// Printing 00:00 on LCD
  LCD_command(0xC0);
  LCD_data(sw_min2);
  LCD_command(0xC1);
  LCD_data(sw_min1);
  LCD_command(0xC2);
  LCD_data(':');
  LCD_command(0xC3);
  LCD_data(sw_sec2);
  LCD_command(0xC4);
  LCD_data(sw_sec1);
  sw_sec1++;
  LCD_command(0xC4);
  LCD_data(sw_sec1);
  if (sw_sec1 == ':') // : is the ASCII for 10
  {
    // Once sec1 reaches 10 turn it to 0 and increment sec2
    sw_sec1 = '0';
    LCD_command(0xC4);
    LCD_data(sw_sec1);
    sw_sec2++;
    LCD_command(0xC3);
    LCD_data(sw_sec2);
  }
  if (sw_sec2 == '6')
  {
    // Once sec2 reaches 6 turn it to 0 and increment min1
    sw_sec2 = '0';
    LCD_command(0xC3);
    LCD_data(sw_sec2);
    sw_min1++;
    LCD_command(0xC1);
    LCD_data(sw_min1);
  }
  if (sw_min1 == ':')
  {
    // Once min1 reaches 10 turn it to 0 and increment min2
    sw_min1 = '0';
    LCD_command(0xC1);
    LCD_data(sw_min1);
    sw_min2++;
    LCD_command(0xC0);
    LCD_data(sw_min2);
  }
  if (sw_min2 == '6' && sw_min1 == '0')
  {
     // When stopwatch finishes reset it
    sw_min2 = '0';
    sw_min1 = '0';
  }
  // Clearing interrupt flag
 TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

/************************************************************************************
*************************************************************************************/

void modes(void)
{
  GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
  mode++;
  if (mode == 4)
    mode = 1;
}

/************************************************************************************
*************************************************************************************/

void timertake(void)
{
  // DISPLAYING TIMER MODE
  LCD_command(0x80);
  LCD_command(1);
  LCD_printString("Timer Mode");
  LCD_command(0xC0);
  LCD_printString("00:00");
  while (mode == 1)
  {
    uint8 key = keypad_read();
    switch (counter)
    {
    case 0:
      // IF A NUMBER IS ENTERED PRESENT IT ON THE SEC1 PLACE ON LCD
      if (isdigit(key))
      {
        sec1 = key;
        LCD_command(0xC4);
        LCD_data(sec1);
        counter++;
        delayMs(500);
      }
      break;
    case 1:

      if (key == '=')
      {
        // IF = IS PRESSED GO TO CASE 5
        counter = 5;
        break;
      }
      else if (isdigit(key))
      {
         // ELSE IF A NUMBER IS PRESSED MOVE SEC1 TO SEC2 AND TAKE NEW SEC1
        sec2 = sec1;
        sec1 = key;
        LCD_command(0xC3);
        LCD_data(sec2);
        LCD_command(0xC4);
        LCD_data(sec1);
        delayMs(500);
        counter++;
      }
      break;
    case 2:

      if (key == '=')
      {
        // IF = IS PRESSED GO TO CASE 5
        counter = 5;
        break;
      }
      else if (isdigit(key))
      {
        // ELSE IF A NUMBER IS PRESSED SHIFT NUMBERS TO LEFT AND TAKE NEW
        min1 = sec2;
        sec2 = sec1;
        sec1 = key;
        LCD_command(0xC1);
        LCD_data(min1);
        LCD_command(0xC3);
        LCD_data(sec2);
        LCD_command(0xC4);
        LCD_data(sec1);
        delayMs(500);
    
        counter++;
        break;
      }
    case 3:

      if (key == '=')
      {
        // IF = IS PRESSED GO TO CASE 5
        counter = 5;
        break;
      }
      else if (isdigit(key))
      {
        // ELSE IF A NUMBER IS PRESSED SHIFT NUMBERS TO LEFT AND TAKE NEW
        min2 = min1;
        min1 = sec2;
        sec2 = sec1;
        sec1 = key;
        LCD_command(0xC0);
        LCD_data(min2);
        LCD_command(0xC1);
        LCD_data(min1);
        LCD_command(0xC3);
        LCD_data(sec2);
        LCD_command(0xC4);
        LCD_data(sec1);
      delayMs(500);
        counter++;
        break;
      }
    case 4:
      counter++;
      break;
    case 5:
      if ((sec2 > '5') || (min2 > '5'))
      {
        // IF NUMBER IS MORE THAN 5 IN SEC 2 OR MIN 2
        // PRINT INVALID INPUT
        // THEN RETURN FROM FUNCTION
        LCD_command(1);
        LCD_printString("INVALID INPUT");
        min2='0';
        min1='0';
        sec2='0';
        sec1='0';
        delayMs(1000);
        counter = 0;
       return;
        break;
      }
      // IF NOT START THE TIMER
      SysTickIntRegister(timerinterrupt);
      SysTickEnable();
      break;
    case 6:
      // CASE USED TO RETURN TIMER TO THE START WHEN TIMER STOPS
      counter = 0;
      SysTickDisable();

      return;
      break;
    default:
      break;
    }
    
  }
}

/************************************************************************************
*************************************************************************************/

void timerinterrupt(void)
{
  // START DECREMENTING THE SECONDS
  sec1--;
  LCD_command(0xC4);
  LCD_data(sec1);
  if (sec1 == '0' && sec2 == '0' && min1 == '0' && min2 == '0')
  {
  // IF TIMER REACHES 0 STOP TIMER AND TOGGLE LED
    SysTickDisable();
    GPIO_PORTF_DATA_R ^= 0x02;
    delayMs(2000);
    GPIO_PORTF_DATA_R ^= 0x02;
    min2 = '0';
    min1 = '0';
    sec2 = '0';
    sec1 = '0';
    counter = 6;
    LCD_command(1);
  }
  if (sec1 == '/') // ASCII FOR NUMBER UNDER 0
  {
    // IF SEC1 REACHS 0 PUT IT AS 9 AND DECREMENT SEC2
    sec1 = '9';
    LCD_command(0xC4);
    LCD_data(sec1);
    sec2--;
    LCD_command(0xC3);
    LCD_data(sec2);
  }

  if (sec2 == '/')
  {
    // IF SEC2 REACHS 0 PUT IT AS 5 AND SEC 1 AS 9 AND DECREMENT MIN1
    sec1 = '9';
    LCD_command(0xC4);
    LCD_data(sec1);
    sec2 = '5';
    LCD_command(0xC3);
    LCD_data(sec2);

    min1--;
    LCD_command(0xC1);
    LCD_data(min1);
  }
  if (min1 == '/')
  {
    // IF MIN1 REACHS 0 PUT IT AS 9 AND SEC1 AS 9 AND SEC2 AS 5 AND DECREMENT MIN2
    sec1 = '9';
    LCD_command(0xC4);
    LCD_data(sec1);
    sec2 = '5';
    LCD_command(0xC3);
    LCD_data(sec2);
    min1 = '9';
    LCD_command(0xC1);
    LCD_data(min1);
    min2--;
    LCD_command(0xC0);
    LCD_data(min2);
  }
}
