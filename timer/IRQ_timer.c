/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../TouchPanel/TouchPanel.h"
#include "../RIT/RIT.h"
#include "stdio.h"
#include "../music/music.h"
#include "../adc/adc.h"

volatile int h;
volatile int m;
volatile int s;
volatile int count;
extern int start_y,start_x_puppy, end_x_puppy, dead;
static int start_y_puppy = (MAX_Y/2)-50;

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
		
	s++;
	count++;
		if(s==60){
			s=0;
			m++;
			if(m==60){
				m=0;
				h++;
			}
		}

		Age(h,m,s);
	
		if(s%2==1)
			Change_Animation();
		else
			Restore_Avatar();
		
		if(count == 5){
			Decrease_Battery();
			count = 0;
		}
	
		
		LPC_TIM0->IR = 1;			/* clear interrupt flag */

  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/




void TIMER1_IRQHandler (void)
{		
	
	disable_timer(2);
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
 
	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
	
  static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = (SinTable[sineticks] * Return_Scaling());  //Return_scaling returns a decimal value in order to increase or decrease the volume
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;



	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER3_IRQHandler (void)
{
	
		/* Match register 0 interrupt service routine */
	if (LPC_TIM3->IR & 01)
	{
		 //Draw heart in the first position
			Draw_Heart(start_x_puppy-10, start_y_puppy-10, Black, Red, White);
		
		
		LPC_TIM3->IR = 1;			/* clear interrupt flag */
	}
		/* Match register 1 interrupt service routine */
	else if(LPC_TIM3->IR & 02)
  {
		
		
		//Delete heart in the first position
		Draw_Heart(start_x_puppy-10, start_y_puppy-10, White, White, White);
		//Draw heart in the first position
		Draw_Heart(start_x_puppy-20, start_y_puppy-20, Black, Red, White);
		
		
		LPC_TIM3->IR =  2 ;			/* clear interrupt flag */	
	}
	/* Match register 2 interrupt service routine */
	else if(LPC_TIM3->IR & 4)
  {
		
		//Delete heart in the second position
		Draw_Heart(start_x_puppy-20, start_y_puppy-20, White, White, White);
		//Draw heart in the third position
		Draw_Heart(start_x_puppy-30, start_y_puppy-30, Black, Red, White);
	
		LPC_TIM3->IR =  4 ;			/* clear interrupt flag */	
	}
	
		/* Match register 3 interrupt service routine */
	else if(LPC_TIM3->IR & 8)
  {
		
	  //Delete heart in the third position
		Draw_Heart(start_x_puppy-30, start_y_puppy-30, White, White, White);
		disable_timer(3);
		Increase_Satiety_Or_Happiness(0);
		
		LPC_TIM3->IR =  8 ;			/* clear interrupt flag */	
	}

	
	return;
}




/******************************************************************************
**                            End Of File
******************************************************************************/
