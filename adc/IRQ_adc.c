/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../GLCD/GLCD.h"
#include "math.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */
volatile int volume = 0;

void ADC_IRQHandler(void) {
  	

  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){
		volume = AD_current *4/0xFFF;
		if(AD_current != 0xFFF && AD_current >50) // se non è il valore massimo o non è nemmeno ad un valore molto basso tale che sfarfalla
		   volume += 1;
		Draw_Volume(volume);
		AD_last = AD_current;
  }
}


float Return_Scaling(void){
  float scaling;
  
	if(AD_current>50 && AD_current<=1025)
    scaling = 0.25;
  else if(AD_current>1025 && AD_current<=2050)
    scaling = 0.5;
  else if(AD_current>2050 && AD_current<=3075)
    scaling = 0.75;
  else if(AD_current>3075&&AD_current<=4096)
    scaling = 1;
  else if(AD_current<=50)
    scaling = 0;
  
    return scaling;

}




