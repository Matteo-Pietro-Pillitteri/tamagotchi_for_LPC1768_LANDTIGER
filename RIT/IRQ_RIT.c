/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../TouchPanel/TouchPanel.h"
#include "../music/music.h"
#include "../adc/adc.h"
#include "stdlib.h"

#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
** 
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int dead,s,m,h;
extern int start_y,start_x_puppy, end_x_puppy;
int sound;
volatile int numSong;
int length=0;
NOTE *notes = NULL;
NOTE select_note = {d5, time_biscroma};
static int current_note=0;

void playSong(NOTE *song, int *sound, int length){
	static int ticks = 0;
	if(length==0){
		return;
	}
	
	if(*sound==1 && !isNotePlaying()){
		++ticks;
		if(ticks== UPTICKS){
			ticks=0;
			playNote(song[current_note++]);
		}
	}
	
	if(current_note>length) {
		*sound=0;
		numSong=0;
		current_note = 0;
		ticks=0;
		disable_timer(2);
		disable_timer(1);
	}	
}

void RIT_IRQHandler (void)
{					
	
	static int select=0;
	static int right = 0; 
	static int left=0;	
	static int type_of_food=0;
	
		/* ADC management */
	ADC_start_conversion();		

	if(numSong==1){
		NOTE cuddle_song[] = {{c6, time_semicroma},{d5, time_biscroma}, {f5, time_semicroma},{c5, time_biscroma},{g5, time_semicroma},{c5, time_biscroma}, {f4, time_semicroma},{d5, time_biscroma}};
		playSong(cuddle_song, &sound, 8);
	}else if(numSong==2){
		NOTE eating_song[] = {{a4, time_biscroma},{cdiesis7, time_biscroma}, {e5, time_biscroma}, {a4, time_semicroma}};
		playSong(eating_song, &sound, 4);
	}else if(numSong==3){
		NOTE death_song[] = {{c3, time_biscroma},{c5, time_biscroma}, {ddiesis5, time_biscroma},{f5, time_biscroma},{fdiesis5, time_biscroma}};
		playSong(death_song, &sound, 5);
	}
	
		
	if(getDisplayPoint(&display, Read_Ads7846(), &matrix ) && dead != 1){ 
		if(display.y > start_y && display.y < MAX_Y - 122)
			if(display.x> start_x_puppy && display.x < end_x_puppy){
				  sound = 1;
				  numSong = 1;
			    enable_timer(3);		
			}
			
	}
	else{
		//do nothing if touch returns values out of bounds
	}

	
	/* 25--> select;   26 --> down;		27 --> left;	 28 --> right;		29 --> up */
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
	
		select++;
		switch(select){
			case 1:
				if(dead != 1){
					/* the age must not increase while the avatar eats. It's not nice that the avatar dies while it eats */
					disable_timer(0);
		      playNote(select_note);
					
					if(type_of_food == 1){
						Draw_Meal(rand()%2);
						Restore_Button_Meal_Color(White, Black);
					}
					else{
						Draw_Snack(rand()%2);
						Restore_Button_Snack_Color(White, Black);
					}
					
					enable_timer(0);
			
			 }else{
					LCD_DrawLine(5, MAX_Y-1, MAX_X-5, MAX_Y-1, White);
					LCD_DrawLine(5, MAX_Y, 5, (MAX_Y - (MAX_Y/5))+5, White);
					LCD_DrawLine(MAX_X-5, MAX_Y, MAX_X-5, (MAX_Y - (MAX_Y/5))+5, White);
					LCD_DrawLine(5, (MAX_Y - (MAX_Y/5))+5, MAX_X-5, (MAX_Y - (MAX_Y/5))+5, White);	
					playNote(select_note);
					End_Screen(White);
					Restore_Buttons_Satiety_Happiness();
					s=0;
					m=0;
				  h=0;
				  Age(s,m,h);
				  Start_Screen();
				  enable_timer(0);
					dead = 0;
			 }
					
				break;
			default:
				break;
		}
	} 
	else{
			select=0;
	}
	
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
	
		left++;
		switch(left){
			case 1:
			if(dead !=1){
					if(type_of_food ==2)
						Restore_Button_Snack_Color(White, Black);
					
					type_of_food = 1; //meal
					Restore_Button_Meal_Color(Red, Red);
						
			}else{
				
					LCD_DrawLine(5, MAX_Y-1, MAX_X-5, MAX_Y-1, Red);
					LCD_DrawLine(5, MAX_Y, 5, (MAX_Y - (MAX_Y/5))+5, Red);
					LCD_DrawLine(MAX_X-5, MAX_Y, MAX_X-5, (MAX_Y - (MAX_Y/5))+5, Red);
					LCD_DrawLine(5, (MAX_Y - (MAX_Y/5))+5, MAX_X-5, (MAX_Y - (MAX_Y/5))+5, Red);	
				
					GUI_Text(((MAX_X)/2) - 13, ((MAX_Y - (MAX_Y/5))+5) + 23, (uint8_t *) "RESET", Red, White);
			}
						
				break;
			default:
				break;
		}
	}
	else{
			left=0;
	}
	
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
	
		right++;
		switch(right){
			case 1:
				if(dead !=1){
						if(type_of_food == 1)
							Restore_Button_Meal_Color(White, Black);
						
						type_of_food = 2; //snack
						Restore_Button_Snack_Color(Red, Red);
						
				}else{
						LCD_DrawLine(5, MAX_Y-1, MAX_X-5, MAX_Y-1, Red);
						LCD_DrawLine(5, MAX_Y, 5, (MAX_Y - (MAX_Y/5))+5, Red);
						LCD_DrawLine(MAX_X-5, MAX_Y, MAX_X-5, (MAX_Y - (MAX_Y/5))+5, Red);
						LCD_DrawLine(5, (MAX_Y - (MAX_Y/5))+5, MAX_X-5, (MAX_Y - (MAX_Y/5))+5, Red);	
					
						GUI_Text(((MAX_X)/2) - 13, ((MAX_Y - (MAX_Y/5))+5) + 23, (uint8_t *) "RESET", Red, White);
				}
			
				break;
			default:
				break;
		}
	}
	else{
			right=0;
		
	}
	
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
