/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.c
** Descriptions:		Has been tested SSD1289、ILI9320、R61505U、SSD1298、ST7781、SPFD5408B、ILI9325、ILI9328、
**						HX8346A、HX8347A
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2012-3-10
** Version:					1.3
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			Paolo Bernardi
** Modified date:		03/01/2020
** Version:					2.0
** Descriptions:		simple arrangement for screen usage
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h" 
#include "AsciiLib.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "stdlib.h"
#include "stdio.h"


/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Code;
static int start_x_animation, start_y_animation, end_y_animation, base_x_animation;
static int start_x, end_x, end_y, base_x, base_y, offset, color1, color2;
static int mouth_start_x, mouth_start_y, mouth_end_x, mouth_end_y;
static int height, width, grade_happiness, grade_satiety;

static int level_x = 10;
static int end_button1 = 110;
static int happiness_x = 25;
static int satiety_x;

/*other*/

static int volume_speaker[16][11]={
{0,0,0,0,0,0,0,0,0,1,1}, 
{0,0,0,0,0,0,0,0,1,1,1}, 
{0,0,0,0,0,0,0,1,1,1,1}, 
{0,0,0,0,0,0,1,1,1,1,1}, 
{0,0,0,0,0,1,1,1,1,1,1}, 
{1,1,1,1,1,1,1,1,1,1,1}, 
{1,1,1,1,1,1,1,1,1,1,1}, 
{1,1,1,1,1,1,1,1,1,1,1}, 
{1,1,1,1,1,1,1,1,1,1,1}, 
{1,1,1,1,1,1,1,1,1,1,1}, 
{1,1,1,1,1,1,1,1,1,1,1}, 
{0,0,0,0,0,1,1,1,1,1,1}, 
{0,0,0,0,0,0,1,1,1,1,1}, 
{0,0,0,0,0,0,0,1,1,1,1}, 
{0,0,0,0,0,0,0,0,1,1,1}, 
{0,0,0,0,0,0,0,0,0,1,1}
};


static int egg[29][30] = {
{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
{0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0},
{0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
{0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
{0,0,0,1,1,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,1,1},
{0,0,1,1,0,0,0,0,0,0,0,0,2,3,3,3,3,3,3,3,2,0,0,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0,2,2,3,3,3,3,3,0,0,0,0,2,0,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0,2,2,3,3,3,3,3,0,0,0,0,2,0,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0,2,2,3,3,3,3,3,3,3,0,0,3,2,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0,2,2,3,3,3,3,3,3,3,3,3,3,3,2,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0,2,2,2,3,3,3,3,3,3,3,3,3,3,2,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,1,1},
{0,1,1,4,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,1,1},
{0,1,1,4,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,1,1},
{0,1,1,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
{0,1,1,1,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,1,1,1,0},
{0,0,0,1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,4,1,1,0,0},
{0,0,0,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,0,0,0,0},
{0,0,0,0,1,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,0,0,0,0},
{0,0,0,0,0,0,0,1,1,1,1,1,1,1,4,4,4,4,4,4,4,4,4,1,1,1,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0}
};

static int pizza[29][29] = {
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,2,2,2,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,2,2,2,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,1,2,2,2,2,1,1,3,3,3,3,3,3,3,3,1,1,0,0,0,0,0},
{0,0,0,0,0,0,1,1,2,2,2,2,1,1,3,3,3,3,3,3,3,3,1,1,0,0,0,0,0},
{0,0,0,0,0,0,1,1,2,2,2,2,1,1,3,3,3,3,3,3,3,3,1,1,0,0,0,0,0},
{0,0,0,0,1,1,2,2,2,2,2,2,1,1,3,3,1,1,1,3,3,3,1,1,0,0,0,0,0},
{0,0,0,0,1,1,2,2,1,1,1,1,3,3,3,1,4,4,4,1,3,3,1,1,0,0,0,0,0},
{0,0,1,1,1,2,2,2,1,1,1,1,3,3,3,1,4,4,4,1,3,3,1,1,0,0,0,0,0},
{0,0,1,1,1,2,2,2,1,1,3,3,3,3,3,1,4,4,4,1,3,3,1,1,0,0,0,0,0},
{0,0,1,1,1,2,2,2,1,1,3,3,3,3,3,3,1,1,1,3,3,3,1,1,0,0,0,0,0},
{0,0,1,1,1,2,2,2,1,1,3,3,3,3,3,3,3,3,3,3,3,3,1,1,0,0,0,0,0},
{1,1,2,2,2,2,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,0,0,0},
{1,1,2,2,2,1,1,3,3,3,3,3,1,1,1,1,3,3,3,3,3,3,3,3,3,1,1,0,0},
{1,1,2,2,2,1,1,3,3,3,3,1,4,4,4,4,1,3,3,3,3,3,3,3,3,1,1,0,0},
{1,1,2,2,2,1,1,3,3,3,3,1,4,4,4,4,1,3,3,3,3,3,3,3,3,1,1,0,0},
{1,1,2,2,2,1,1,3,3,3,3,1,4,4,4,4,1,3,3,3,3,3,3,3,3,1,1,0,0},
{1,1,2,2,2,1,1,3,3,3,3,1,4,4,4,4,1,3,3,3,3,3,3,3,3,1,1,0,0},
{1,1,1,1,1,1,1,3,3,3,3,3,1,1,1,1,3,3,3,3,3,3,3,3,3,1,1,0,0},
{1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,3,3,1,1},
{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,3,3,3,1,4,4,4,4,1,3,1,1},
{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,3,3,3,1,4,4,4,4,1,3,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,4,4,4,4,1,3,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,3,3,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
};

static int lolly_pop[30][21] = {
{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
{0,0,0,1,4,4,4,6,6,6,6,6,6,6,6,4,4,1,0,0,0},
{0,1,1,1,4,4,4,6,6,6,6,6,6,6,6,4,4,1,1,1,0},
{1,1,4,4,4,4,4,4,6,6,6,6,6,6,4,4,4,4,4,1,1},
{1,1,4,4,4,4,4,4,6,6,6,6,6,4,4,4,4,4,4,1,1},
{1,4,4,4,4,4,4,4,4,6,6,6,4,4,4,4,4,4,4,4,1},
{1,6,6,4,4,4,4,4,4,6,6,4,4,4,4,4,4,4,4,4,1},
{1,6,6,6,6,4,4,4,4,4,4,4,4,4,4,4,6,6,6,6,1},
{1,6,6,6,6,6,6,6,6,4,4,4,6,6,6,6,6,6,6,6,1},
{1,6,6,6,6,6,6,6,6,4,4,4,6,6,6,6,6,6,6,6,1},
{1,6,6,6,6,4,4,4,4,4,4,4,4,6,6,6,6,6,6,6,1},
{1,6,6,4,4,4,4,4,4,4,6,4,4,4,4,6,6,6,6,6,1},
{1,4,4,4,4,4,4,4,4,6,6,6,4,4,4,4,4,4,4,1,1},
{1,1,4,4,4,4,4,4,6,6,6,6,6,4,4,4,4,4,4,1,1},
{0,1,1,1,4,4,4,6,6,6,6,6,6,4,4,4,4,1,1,1,0},
{0,0,0,1,4,4,4,6,6,6,6,6,6,6,4,4,4,1,0,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,5,5,5,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0}
};

static int coke[37][23] = {
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,1,0},
{1,1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,1,1},
{1,1,5,5,5,5,1,0,0,0,0,1,5,5,5,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,1,0,0,0,0,1,5,5,5,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,1,0,0,0,0,1,5,5,5,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,1,0,0,0,0,1,5,5,5,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,1,0,0,0,0,1,5,5,5,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,1,0,0,0,0,1,1,1,1,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,1,0,0,0,0,0,0,0,1,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,1,1,1,0,0,0,0,0,1,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,5,5,1,0,0,0,0,0,1,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,5,5,1,0,0,0,0,0,1,5,5,5,5,5,5,1,1},
{1,1,5,5,5,5,5,5,1,0,0,0,0,0,1,1,1,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,1,1,1,0,0,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,1,0,0,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,1,0,0,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,1,0,0,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,1,1,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,5,5,5,5,5,5,5,5,5,5,1,0,0,0,0,1,5,5,5,1,1},
{1,1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,1,1},
{1,1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,1,1},
{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0}
};

/* heart matrix for animation in timer3 */
static int heart[15][16] = {
{0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
{0,0,1,2,2,2,1,0,0,1,2,2,2,1,0,0},
{0,1,2,2,2,2,2,1,1,2,2,2,2,2,1,0},
{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
{0,1,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
{0,1,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
{0,0,1,2,2,2,2,2,2,2,2,2,2,1,0,0},
{0,0,0,1,2,2,2,2,2,2,2,2,1,0,0,0},
{0,0,0,0,1,2,2,2,2,2,2,1,0,0,0,0},
{0,0,0,0,0,1,2,2,2,2,1,0,0,0,0,0},
{0,0,0,0,0,0,1,2,2,1,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
};


volatile int dead = 0;
volatile int dead_song = 0;
volatile int start_x_puppy, end_x_puppy, start_y;
extern int sound, numSong, flag_select, volume;


/* Private define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */
#define  HX8346A		10 /* 0x0046 */  
#define  HX8347D    11 /* 0x0047 */
#define  HX8347A    12 /* 0x0047 */	
#define  LGDP4535   13 /* 0x4535 */  
#define  SSD2119    14 /* 3.5 LCD 0x9919 */

/*******************************************************************************
* Function Name  : Lcd_Configuration
* Description    : Configures LCD Control lines
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Configuration(void)
{
	/* Configure the LCD Control pins */
	
	/* EN = P0.19 , LE = P0.20 , DIR = P0.21 , CS = P0.22 , RS = P0.23 , RS = P0.23 */
	/* RS = P0.23 , WR = P0.24 , RD = P0.25 , DB[0.7] = P2.0...P2.7 , DB[8.15]= P2.0...P2.7 */  
	LPC_GPIO0->FIODIR   |= 0x03f80000;
	LPC_GPIO0->FIOSET    = 0x03f80000;
}

/*******************************************************************************
* Function Name  : LCD_Send
* Description    : LCD写数据
* Input          : - byte: byte to be sent
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_Send (uint16_t byte) 
{
	LPC_GPIO2->FIODIR |= 0xFF;          /* P2.0...P2.7 Output */
	LCD_DIR(1)		   				    				/* Interface A->B */
	LCD_EN(0)	                        	/* Enable 2A->2B */
	LPC_GPIO2->FIOPIN =  byte;          /* Write D0..D7 */
	LCD_LE(1)                         
	LCD_LE(0)														/* latch D0..D7	*/
	LPC_GPIO2->FIOPIN =  byte >> 8;     /* Write D8..D15 */
}

/*******************************************************************************
* Function Name  : wait_delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None 
*******************************************************************************/
static void wait_delay(int count)
{
	while(count--);
}

/*******************************************************************************
* Function Name  : LCD_Read
* Description    : LCD读数据
* Input          : - byte: byte to be read
* Output         : None
* Return         : 返回读取到的数据
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_Read (void) 
{
	uint16_t value;
	
	LPC_GPIO2->FIODIR &= ~(0xFF);              /* P2.0...P2.7 Input */
	LCD_DIR(0);		   				           				 /* Interface B->A */
	LCD_EN(0);	                               /* Enable 2B->2A */
	wait_delay(30);							   						 /* delay some times */
	value = LPC_GPIO2->FIOPIN0;                /* Read D8..D15 */
	LCD_EN(1);	                               /* Enable 1B->1A */
	wait_delay(30);							   						 /* delay some times */
	value = (value << 8) | LPC_GPIO2->FIOPIN0; /* Read D0..D7 */
	LCD_DIR(1);
	return  value;
}

/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Description    : LCD写寄存器地址
* Input          : - index: 寄存器地址
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteIndex(uint16_t index)
{
	LCD_CS(0);
	LCD_RS(0);
	LCD_RD(1);
	LCD_Send( index ); 
	wait_delay(22);	
	LCD_WR(0);  
	wait_delay(1);
	LCD_WR(1);
	LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : LCD写寄存器数据
* Input          : - index: 寄存器数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteData(uint16_t data)
{				
	LCD_CS(0);
	LCD_RS(1);   
	LCD_Send( data );
	LCD_WR(0);     
	wait_delay(1);
	LCD_WR(1);
	LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 读取控制器数据
* Input          : None
* Output         : None
* Return         : 返回读取到的数据
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_ReadData(void)
{ 
	uint16_t value;
	
	LCD_CS(0);
	LCD_RS(1);
	LCD_WR(1);
	LCD_RD(0);
	value = LCD_Read();
	
	LCD_RD(1);
	LCD_CS(1);
	
	return value;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{ 
	/* Write 16-bit Index, then Write Reg */  
	LCD_WriteIndex(LCD_Reg);         
	/* Write 16-bit Reg */
	LCD_WriteData(LCD_RegValue);  
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	uint16_t LCD_RAM;
	
	/* Write 16-bit Index (then Read Reg) */
	LCD_WriteIndex(LCD_Reg);
	/* Read 16-bit Reg */
	LCD_RAM = LCD_ReadData();      	
	return LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_SetCursor(uint16_t Xpos,uint16_t Ypos)
{
    #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )
	
 	uint16_t temp = Xpos;

			 Xpos = Ypos;
			 Ypos = ( MAX_X - 1 ) - temp;  

	#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )
		
	#endif

  switch( LCD_Code )
  {
     default:		 /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
          LCD_WriteReg(0x0020, Xpos );     
          LCD_WriteReg(0x0021, Ypos );     
	      break; 

     case SSD1298: 	 /* 0x8999 */
     case SSD1289:   /* 0x8989 */
	      LCD_WriteReg(0x004e, Xpos );      
          LCD_WriteReg(0x004f, Ypos );          
	      break;  

     case HX8346A: 	 /* 0x0046 */
     case HX8347A: 	 /* 0x0047 */
     case HX8347D: 	 /* 0x0047 */
	      LCD_WriteReg(0x02, Xpos>>8 );                                                  
	      LCD_WriteReg(0x03, Xpos );  

	      LCD_WriteReg(0x06, Ypos>>8 );                           
	      LCD_WriteReg(0x07, Ypos );    
	
	      break;     
     case SSD2119:	 /* 3.5 LCD 0x9919 */
	      break; 
  }
}

/*******************************************************************************
* Function Name  : LCD_Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1141; j++ );
	}
} 


/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : Initialize TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Initialization(void)
{
	uint16_t DeviceCode;
	
	LCD_Configuration();
	delay_ms(100);
	DeviceCode = LCD_ReadReg(0x0000);		/* 读取屏ID	*/	
	
	if( DeviceCode == 0x9325 || DeviceCode == 0x9328 )	
	{
		LCD_Code = ILI9325;
		LCD_WriteReg(0x00e7,0x0010);      
		LCD_WriteReg(0x0000,0x0001);  	/* start internal osc */
		LCD_WriteReg(0x0001,0x0100);     
		LCD_WriteReg(0x0002,0x0700); 	/* power on sequence */
		LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3) ); 	/* importance */
		LCD_WriteReg(0x0004,0x0000);                                   
		LCD_WriteReg(0x0008,0x0207);	           
		LCD_WriteReg(0x0009,0x0000);         
		LCD_WriteReg(0x000a,0x0000); 	/* display setting */        
		LCD_WriteReg(0x000c,0x0001);	/* display setting */        
		LCD_WriteReg(0x000d,0x0000); 			        
		LCD_WriteReg(0x000f,0x0000);
		/* Power On sequence */
		LCD_WriteReg(0x0010,0x0000);   
		LCD_WriteReg(0x0011,0x0007);
		LCD_WriteReg(0x0012,0x0000);                                                                 
		LCD_WriteReg(0x0013,0x0000);                 
		delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0010,0x1590);   
		LCD_WriteReg(0x0011,0x0227);
		delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0012,0x009c);                  
		delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0013,0x1900);   
		LCD_WriteReg(0x0029,0x0023);
		LCD_WriteReg(0x002b,0x000e);
		delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0020,0x0000);                                                            
		LCD_WriteReg(0x0021,0x0000);           
		delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0030,0x0007); 
		LCD_WriteReg(0x0031,0x0707);   
		LCD_WriteReg(0x0032,0x0006);
		LCD_WriteReg(0x0035,0x0704);
		LCD_WriteReg(0x0036,0x1f04); 
		LCD_WriteReg(0x0037,0x0004);
		LCD_WriteReg(0x0038,0x0000);        
		LCD_WriteReg(0x0039,0x0706);     
		LCD_WriteReg(0x003c,0x0701);
		LCD_WriteReg(0x003d,0x000f);
		delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0050,0x0000);        
		LCD_WriteReg(0x0051,0x00ef);   
		LCD_WriteReg(0x0052,0x0000);     
		LCD_WriteReg(0x0053,0x013f);
		LCD_WriteReg(0x0060,0xa700);        
		LCD_WriteReg(0x0061,0x0001); 
		LCD_WriteReg(0x006a,0x0000);
		LCD_WriteReg(0x0080,0x0000);
		LCD_WriteReg(0x0081,0x0000);
		LCD_WriteReg(0x0082,0x0000);
		LCD_WriteReg(0x0083,0x0000);
		LCD_WriteReg(0x0084,0x0000);
		LCD_WriteReg(0x0085,0x0000);
		  
		LCD_WriteReg(0x0090,0x0010);     
		LCD_WriteReg(0x0092,0x0000);  
		LCD_WriteReg(0x0093,0x0003);
		LCD_WriteReg(0x0095,0x0110);
		LCD_WriteReg(0x0097,0x0000);        
		LCD_WriteReg(0x0098,0x0000);  
		/* display on sequence */    
		LCD_WriteReg(0x0007,0x0133);
		
		LCD_WriteReg(0x0020,0x0000);  /* 行首址0 */                                                          
		LCD_WriteReg(0x0021,0x0000);  /* 列首址0 */     
	}

    delay_ms(50);   /* delay 50 ms */	
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index;
	
	if( LCD_Code == HX8347D || LCD_Code == HX8347A )
	{
		LCD_WriteReg(0x02,0x00);                                                  
		LCD_WriteReg(0x03,0x00);  
		                
		LCD_WriteReg(0x04,0x00);                           
		LCD_WriteReg(0x05,0xEF);  
		                 
		LCD_WriteReg(0x06,0x00);                           
		LCD_WriteReg(0x07,0x00);    
		               
		LCD_WriteReg(0x08,0x01);                           
		LCD_WriteReg(0x09,0x3F);     
	}
	else
	{	
		LCD_SetCursor(0,0); 
	}	

	LCD_WriteIndex(0x0022);
	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
		LCD_WriteData(Color);
	}
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* Input          : - color: BRG 颜色值  
* Output         : None
* Return         : RGB 颜色值
* Attention		 : 内部函数调用
*******************************************************************************/
static uint16_t LCD_BGR2RGB(uint16_t color)
{
	uint16_t  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : 获取指定座标的颜色值
* Input          : - Xpos: Row Coordinate
*                  - Xpos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
	uint16_t dummy;
	
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteIndex(0x0022);  
	
	switch( LCD_Code )
	{
		case ST7781:
		case LGDP4531:
		case LGDP4535:
		case SSD1289:
		case SSD1298:
             dummy = LCD_ReadData();   /* Empty read */
             dummy = LCD_ReadData(); 	
 		     return  dummy;	      
	    case HX8347A:
	    case HX8347D:
             {
		        uint8_t red,green,blue;
				
				dummy = LCD_ReadData();   /* Empty read */

		        red = LCD_ReadData() >> 3; 
                green = LCD_ReadData() >> 2; 
                blue = LCD_ReadData() >> 3; 
                dummy = (uint16_t) ( ( red<<11 ) | ( green << 5 ) | blue ); 
		     }	
	         return  dummy;

        default:	/* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x9919 */
             dummy = LCD_ReadData();   /* Empty read */
             dummy = LCD_ReadData(); 	
 		     return  LCD_BGR2RGB( dummy );
	}
}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 在指定座标画点
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: A点行座标
*                  - y1: A点列座标 
*				   - x2: B点行座标
*				   - y2: B点列座标 
*				   - color: 线颜色
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;      /* 定义X Y轴上增加的变量值 */
    short temp;       /* 起点 终点大小比较 交换数据时的中间变量 */

    if( x0 > x1 )     /* X轴上起点大于终点 交换数据 */
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;   
    }
    if( y0 > y1 )     /* Y轴上起点大于终点 交换数据 */
    {
		temp = y1;
		y1 = y0;
		y0 = temp;   
    }
  
	dx = x1-x0;       /* X轴方向上的增量 */
	dy = y1-y0;       /* Y轴方向上的增量 */

    if( dx == 0 )     /* X轴上没有增量 画垂直线 */ 
    {
        do
        { 
            LCD_SetPoint(x0, y0, color);   /* 逐点显示 描垂直线 */
            y0++;
        }
        while( y1 >= y0 ); 
		return; 
    }
    if( dy == 0 )     /* Y轴上没有增量 画水平直线 */ 
    {
        do
        {
            LCD_SetPoint(x0, y0, color);   /* 逐点显示 描水平线 */
            x0++;
        }
        while( x1 >= x0 ); 
		return;
    }
	/* 布兰森汉姆(Bresenham)算法画线 */
    if( dx > dy )                         /* 靠近X轴 */
    {
	    temp = 2 * dy - dx;               /* 计算下个点的位置 */         
        while( x0 != x1 )
        {
	        LCD_SetPoint(x0,y0,color);    /* 画起点 */ 
	        x0++;                         /* X轴上加1 */
	        if( temp > 0 )                /* 判断下下个点的位置 */
	        {
	            y0++;                     /* 为右上相邻点，即（x0+1,y0+1） */ 
	            temp += 2 * dy - 2 * dx; 
	 	    }
            else         
            {
			    temp += 2 * dy;           /* 判断下下个点的位置 */  
			}       
        }
        LCD_SetPoint(x0,y0,color);
    }  
    else
    {
	    temp = 2 * dx - dy;                      /* 靠近Y轴 */       
        while( y0 != y1 )
        {
	 	    LCD_SetPoint(x0,y0,color);     
            y0++;                 
            if( temp > 0 )           
            {
                x0++;               
                temp+=2*dy-2*dx; 
            }
            else
			{
                temp += 2 * dy;
			}
        } 
        LCD_SetPoint(x0,y0,color);
	}
} 

/******************************************************************************
* Function Name  : PutChar
* Description    : 将Lcd屏上任意位置显示一个字符
* Input          : - Xpos: 水平坐标 
*                  - Ypos: 垂直坐标  
*				   - ASCI: 显示的字符
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* 取字模数据 */
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( ((tmp_char >> (7 - j)) & 0x01) == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* 字符颜色 */
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* 背景颜色 */
            }
        }
    }
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : 在指定座标显示字符串
* Input          : - Xpos: 行座标
*                  - Ypos: 列座标 
*				   - str: 字符串
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}

/* MY FUNCTIONS*/ 

void Draw_Volume(uint16_t volume){

	switch(volume){
		case 1:
				LCD_DrawRectangle(18, 8 , 21 , 20, Green);
				LCD_DrawRectangle(24, 8 , 27 , 20, White);
				LCD_DrawRectangle(30, 8 , 33 , 20, White);
		break;
		
		case 2:
				LCD_DrawRectangle(18, 8 , 21 , 20, Green);
				LCD_DrawRectangle(24, 8 , 27 , 20, Yellow);
				LCD_DrawRectangle(30, 8 , 33 , 20, White);
		break;
		
		case 3:
				LCD_DrawRectangle(18, 8 , 21 , 20, Green);
				LCD_DrawRectangle(24, 8 , 27 , 20, Yellow);
				LCD_DrawRectangle(30, 8 , 33 , 20, Red);
		break;
		
		case 0:
				LCD_DrawRectangle(18, 8 , 21 , 20, White);
				LCD_DrawRectangle(24, 8 , 27 , 20, White);
				LCD_DrawRectangle(30, 8 , 33 , 20, White);
			break;
	}
   
}

void first_rectangle_coordinates(void){
		base_x = (MAX_X/2)-16;
		base_y = (MAX_Y/2)-41;
		start_x = base_x;
		start_y = base_y;
		end_x = (MAX_X/2)+16;
		end_y = base_y+6; // rettancle height is 6
}

void Draw_Eating_Mouth(uint16_t color1, uint16_t color2){
		uint16_t startx_dinamic_mouth = mouth_start_x + 12;
		uint16_t starty_dinamic_mouth = mouth_end_y - 6;
		
		LCD_DrawRectangle(startx_dinamic_mouth+6, starty_dinamic_mouth, startx_dinamic_mouth + 18, starty_dinamic_mouth + 6, color1);
		LCD_DrawRectangle(startx_dinamic_mouth, starty_dinamic_mouth + 6, startx_dinamic_mouth + 6, starty_dinamic_mouth + 12, color1);
		LCD_DrawRectangle(startx_dinamic_mouth + 6, starty_dinamic_mouth + 6, startx_dinamic_mouth + 12, starty_dinamic_mouth + 12, color2);
		LCD_DrawRectangle(startx_dinamic_mouth + 12, starty_dinamic_mouth + 6, startx_dinamic_mouth + 18, starty_dinamic_mouth + 12, color2);
		LCD_DrawRectangle(startx_dinamic_mouth + 18, starty_dinamic_mouth + 6, startx_dinamic_mouth + 24, starty_dinamic_mouth + 12, color1);
		LCD_DrawRectangle(startx_dinamic_mouth+6, starty_dinamic_mouth + 12, startx_dinamic_mouth + 18, starty_dinamic_mouth + 18, color1);
		
}


void Draw_Static_Mouth(uint16_t s_x, uint16_t s_y, uint16_t e_x, uint16_t e_y, uint16_t color2){
	short i;
	
	//mouth	
		for(i=0;i<5;i++){
				
			s_x = e_x;
			
			if(i==0)
				s_y = e_y;
		
			e_x += 6;
			
			if(i%2 != 0){
				s_y = e_y + 6;
				e_y = s_y + 10;
			}
			else{
				s_y = e_y - 6;
				e_y = s_y - 10;
			}
			
			LCD_DrawRectangle(s_x, s_y, e_x, e_y, color2);
		}
}

void Draw_Puppy(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t base_x, uint16_t color1, uint16_t color2){
	int i;
	
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		
		start_x = end_x;
		start_y = end_y;
	
		end_x += 10;
		end_y += 6;
	
		offset = end_x - start_x;
	
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		LCD_DrawRectangle(base_x, start_y, (base_x-offset), end_y, color1);
	
		base_x -= offset;
		
		start_x = end_x;
		start_y = end_y;
	
		end_x += 6;
		end_y += 6;
		
		offset = end_x - start_x;
		

		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		LCD_DrawRectangle(base_x, start_y, (base_x-offset), end_y, color1);
		
		base_x -= offset;
		
		start_x = end_x;
		start_y = end_y;
		
		end_x += 6;
		end_y += 10;
		
		offset = end_x - start_x;
		
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		LCD_DrawRectangle(base_x, start_y, (base_x-offset), end_y, color1);
		
		for(i=0;i<2;i++){
			base_x -= offset;
			
			
			start_x = end_x;
			start_y = end_y;
			
			end_x += 6;
			end_y += 15;
			end_x_puppy = end_x;
	
			LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);

			if(i<1){
					start_x_animation = start_x;
				  start_y_animation = start_y;
				  end_y_animation = end_y;
				  base_x_animation = base_x;

					// eyes
					LCD_DrawRectangle(base_x+12, start_y-6, base_x+18, end_y-12, color1);
					LCD_DrawRectangle(base_x+26, start_y-6, base_x+32, end_y-12, color1);
				
					//hand
					LCD_DrawRectangle(start_x-12, start_y, start_x-24, end_y-9, color1);
					LCD_DrawRectangle(start_x-6, start_y+6, start_x-12, end_y, color1);
					LCD_DrawRectangle(start_x-12, end_y, start_x-18, end_y+6, color1);
					LCD_DrawRectangle(start_x-24, start_y+6, start_x-30, start_y+12, color1);
					
					//cont to draw face
					offset = end_x - start_x;
					LCD_DrawRectangle(base_x, start_y, (base_x-offset), (end_y+10), color1);	
				  start_x_puppy = base_x - offset;
			}
			
		}
		
		start_y = end_y;
		
		end_x = start_x -6;
		end_y += 6;
		
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);


		
		for(i=0;i<2;i++){
		
		start_x = end_x;
		start_y = end_y;
		
		end_x -= 10;
		end_y += 6;
		
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		
		}
		
		start_x = end_x;
		start_y = end_y;
		
		end_x -= 26;
		end_y += 6;
	
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		
		start_x = end_x;
		start_y = end_y -6;
		
		end_x -= 17;
		end_y -= 12;
	
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		
		start_x = end_x;
		start_y = end_y;
		
		end_x -= 6;
		end_y -= 6;
	
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		
		start_x = end_x;
		start_y = end_y;
		
		end_x -= 6;
		end_y -= 10;
	
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color1);
		
		mouth_start_x = start_x; 
		mouth_start_y = start_y; 
		mouth_end_x = end_x;
		mouth_end_y = end_y;
		
		Draw_Static_Mouth(mouth_start_x, mouth_start_y, mouth_end_x, mouth_end_y, color2);
}

void delete_food(int dim_x, int dim_y){
	int i,y;

	//delete static mouth
	Draw_Static_Mouth(mouth_start_x, mouth_start_y, mouth_end_x, mouth_end_y, White);
	
	//Draw eating mouth
	Draw_Eating_Mouth(Red, Black);
	
	
	for(i=0;i<dim_y;i++)
		for(y=0;y<dim_x;y++)
			LCD_SetPoint(y+20,i+175, White);
	
		
	//Delete Eating_mouth
	Draw_Eating_Mouth(White, White);
	
	//Draw static mouth
	Draw_Static_Mouth(mouth_start_x, mouth_start_y, mouth_end_x, mouth_end_y, Red);
	
}

void Draw_VolumeSpeaker(void){
	int i, y;
	height = 16;
	width = 11;
	
	for(i=0;i<height;i++)
			for(y=0;y<width;y++){
				switch(volume_speaker[i][y]){
					case 1:
							LCD_SetPoint(y+6, i+6, Black);
					break;
				
					default:
							LCD_SetPoint(y+6, i+6, White);
					break;
				}
			}
			
			LCD_DrawRectangle(18, 8 , 21 , 20, Green);
			LCD_DrawRectangle(24, 8 , 27 , 20, Yellow);
			LCD_DrawRectangle(30, 8 , 33 , 20, Red);
}
				

void Draw_Heart(int offset_x,int offset_y, uint16_t color1, uint16_t color2, uint16_t color3){
	int i, y;
  height = 15;
  width = 16;
	
		for(i=0;i<height;i++)
			for(y=0;y<width;y++){
				switch(heart[i][y]){
					case 1:
							LCD_SetPoint(y+offset_x, i+offset_y, color1);
					break;
						
					case 2:
							LCD_SetPoint(y+offset_x, i+offset_y, color2);
					break;
					
					default:
							LCD_SetPoint(y+offset_x, i+offset_y, color3);
					break;
				}
			}
		
}


int Eating(int dim_x, int dim_y ){
		int i;

		sound = 1;
		numSong = 2;
	  disable_RIT(); /* Food selection during eating is disabled.*/
		Restore_Avatar();
	  first_rectangle_coordinates();

		for(i=0;i<3;i++){

			color1 = White;
			color2 = White;
	
			Draw_Puppy(start_x, start_y, end_x, end_y, base_x, color1, color2);
			
			start_x -= 10;
			base_x -= 10;
			end_x -= 10;
			color1 = Black;
			color2 = Red;
			
			Draw_Puppy(start_x, start_y, end_x, end_y, base_x, color1, color2);
		}
		
	 delete_food(dim_x, dim_y);
		
		for(i=0;i<3;i++){
			color1 = White;
			color2 = White;
	
			Draw_Puppy(start_x, start_y, end_x, end_y, base_x, color1, color2);
			
			base_x += 10;
			start_x += 10;
			end_x += 10;
			color1 = Black;
			color2 = Red;
			
			Draw_Puppy(start_x, start_y, end_x, end_y, base_x, color1, color2);
		}
		
			enable_RIT();
		
	return 1;
			
}

void Increase_Satiety_Or_Happiness(int S_or_H){
		
		if(S_or_H == 1){
					LCD_DrawRectangle(((grade_satiety*level_x)+(satiety_x + 5))+2, (MAX_Y/7)+2, ((satiety_x + 5) + (grade_satiety*level_x) + level_x)-2, (MAX_Y/5)-2, Green);
					grade_satiety++;
		}else{
				if(grade_happiness != 5){
					LCD_DrawRectangle(((grade_happiness*level_x)+(happiness_x + 5))+2, (MAX_Y/7)+2, ((happiness_x + 5) + (grade_happiness*level_x) + level_x)-2, (MAX_Y/5)-2, Green);
					grade_happiness++;
				}
		}
				
		
}

void Draw_Lolly_Pop(void){
	int i, y;
	height = 30;
	width = 22;
	
	for(i=0;i<height;i++)
			for(y=0;y<width;y++){
				switch(lolly_pop[i][y]){
					case 1:
							LCD_SetPoint(y+20, i+175, Black);
					break;
						
					case 4:
							LCD_SetPoint(y+20, i+175, Blue);
					break;
					
					case 5:
							LCD_SetPoint(y+20, i+175, RGB565CONVERT(179,121,55));
					break;
					
					case 6:
							LCD_SetPoint(y+20, i+175, RGB565CONVERT(47, 181, 238)); // light blue
					break;
						
					default:
							LCD_SetPoint(y+20, i+175, White);
					break;
				}
			}
			
			if(Eating(width, height) && grade_happiness != 5) Increase_Satiety_Or_Happiness(0);
	
}


void Draw_Coke(void){
	int i, y;
  height = 37;
  width = 23;
	
		for(i=0;i<height;i++)
			for(y=0;y<width;y++){
				switch(coke[i][y]){
					case 1:
							LCD_SetPoint(y+20, i+175, Black);
					break;
						
					case 5:
							LCD_SetPoint(y+20, i+175, Red);
					break;
					
					case 7:
							LCD_SetPoint(y+20, i+175, Blue);
					break;
						
					default:
							LCD_SetPoint(y+20, i+175, White);
					break;
				}
			}
			
		if(Eating(width, height) && grade_happiness != 5) Increase_Satiety_Or_Happiness(0);
		
}

void Draw_Pizza(void){
	int i, y;
	height = 29;
	width = 29;
	
	for(i=0;i<height; i++)
		for(y=0; y<width; y++){
				switch(pizza[i][y]){
					case 1:
						LCD_SetPoint(y+20,i+175, Black);
					break;
						
					case 2:
						LCD_SetPoint(y+20,i+175, RGB565CONVERT(146,72,23)); //Brown
					break;
						
					case 3:
						LCD_SetPoint(y+20,i+175, RGB565CONVERT(254,226,88)); //simpson 
					break;
					
					case 4:
						LCD_SetPoint(y+20,i+175, Red);
					break;
					
					default:
						LCD_SetPoint(y+20,i+175, White);
					break;
				}
		}			
		
			if(Eating(width, height) && grade_satiety != 5) Increase_Satiety_Or_Happiness(1);
}

void Draw_Egg(void){
	int i, y;
	height = 29;
	width = 30;
	
	for(i=0;i<height; i++)
		for(y=0; y<width; y++){
				switch(egg[i][y]){
					case 1:
						LCD_SetPoint(y+20,i+175, Black);
					break;
						
					case 2:
						LCD_SetPoint(y+20,i+175, RGB565CONVERT(240,96,0));
					break;
						
					case 3:
						LCD_SetPoint(y+20,i+175, Yellow);
					break;
					
					case 4:
						LCD_SetPoint(y+20,i+175, Grey);
					
					default:
						LCD_SetPoint(y+20,i+175, White);
					break;
					
				}
		}			
		
			if(Eating(width, height) && grade_satiety != 5)Increase_Satiety_Or_Happiness(1);
}

void Draw_Meal(int chosen){
	

	
	switch(chosen){
		case 0:
			Draw_Egg();
		break;
		
		case 1:
			Draw_Pizza();
		break;
	}
}

void Draw_Snack(int chosen){
	
	
	switch(chosen){
		case 0:
			Draw_Coke();
		break;
		
		case 1:
			Draw_Lolly_Pop();
		break;
	}
	
}


void FullOfColour(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color){
	int i, y;
	short temp;
	
	 if(x0> x1){
			temp = x1;
			x1 = x0;
			x0 = temp;   
		}
	 
		 if( y0 > y1 ){
			temp = y1;
			y1 = y0;
			y0 = temp;   
    }
	 
	for(i=x0; i<x1; i++)
		for(y=y0; y<y1; y++)
			LCD_SetPoint(i,y,color);
		
}

void LCD_DrawRectangle (uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color){
	
		LCD_DrawLine(x0, y1, x1, y1, color);
		LCD_DrawLine(x0, y1, x0, y0, color);
		LCD_DrawLine(x1, y1, x1, y0, color);
		LCD_DrawLine(x0, y0, x1, y0, color);
	
		FullOfColour(x0, y0, x1, y1, color);
}

void Charge_Satiety_Happiness(){
	int i;
	
	for(i=0;i<5;i++){
		LCD_DrawRectangle((i*level_x)+(happiness_x + 5), MAX_Y/7, (happiness_x + 5) + (i*level_x) + level_x, (MAX_Y/5), White);
		LCD_DrawRectangle(((i*level_x)+(happiness_x + 5))+2, (MAX_Y/7)+2, ((happiness_x + 5) + (i*level_x) + level_x)-2, (MAX_Y/5)-2, Green);
		
		LCD_DrawRectangle((i*level_x)+(satiety_x + 5), MAX_Y/7, (satiety_x + 5) + (i*level_x) + level_x, (MAX_Y/5), White);
		LCD_DrawRectangle(((i*level_x)+(satiety_x + 5))+2, (MAX_Y/7)+2, ((satiety_x + 5) + (i*level_x) + level_x)-2, (MAX_Y/5)-2, Green);
	}
	grade_happiness = i;
	grade_satiety = i;
}


void Restore_Buttons_Satiety_Happiness(void){
	
		GUI_Text(((MAX_X)/2) - 13, ((MAX_Y - (MAX_Y/5))+5) + 23, (uint8_t *) "RESET", White, White);  
		LCD_DrawRectangle(end_button1, (MAX_Y - (MAX_Y/5)) + 5 , end_button1 + 10, MAX_Y ,Black);
	
		GUI_Text(38, ((MAX_Y - (MAX_Y/5))+5) + 23, (uint8_t *) "MEAL", Black, White);
		GUI_Text((end_button1+10)+38,(((MAX_Y - (MAX_Y/5))+5))+23, (uint8_t *) "SNACK", Black, White);
	
		Charge_Satiety_Happiness();
	
}

void End_Screen(uint16_t color){
		int i, b_h = 6; // b_h -> base height of a rectangle
	
		start_x = MAX_X/2 - 30;
		start_y = MAX_Y/2 -27;
		end_x = MAX_X/2 + 30;
		end_y = start_y + 6;
	
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color);
		start_y = end_y;
		end_y += b_h;
	
		LCD_DrawRectangle(start_x, start_y, start_x - b_h, end_y, color);
		LCD_DrawRectangle(end_x, start_y, end_x + b_h, end_y, color);
		
		start_y = end_y;
		end_y += (b_h*5);
	
		LCD_DrawRectangle(start_x - b_h, start_y, start_x-(b_h*2), end_y, color);
		LCD_DrawRectangle(end_x + b_h, start_y, end_x + (b_h*2), end_y, color);
		
		start_y = end_y;
		end_y += b_h;
		
		LCD_DrawRectangle(start_x - b_h, start_y, start_x, end_y, color);
		LCD_DrawRectangle(end_x+ b_h, start_y, end_x, end_y, color);
		
		start_y = end_y;
		end_y += b_h;
		
		LCD_DrawRectangle(start_x, start_y, start_x + (b_h*3), end_y, color);
		LCD_DrawRectangle(start_x + (b_h*5), start_y, end_x, end_y, color);
		start_y = end_y;
		end_y += b_h;

		for(i=2; i<5; i++){
			LCD_DrawRectangle(start_x + (i*b_h), start_y, start_x + ((i+1)*b_h), end_y, color);		
			
			start_y = end_y;
			end_y += b_h;
		}
		
		end_y -= (b_h*4);
		
		LCD_DrawRectangle(start_x + (5*b_h), start_y, start_x + (6*b_h), end_y, color);

		start_x = MAX_X/2 - 15;
		start_y = MAX_Y/2 - (b_h*2);
		end_x = start_x + b_h;
		end_y = start_y + b_h;
		
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color);
		
		start_y += (b_h*2);
		end_y = start_y + b_h;
		
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color);
		
		start_y -= (b_h*2);
		start_x += (b_h*2);
		end_x += (b_h*2);
	
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color);
		
		start_x += b_h;
		end_x += b_h;
		start_y = end_y;
		end_y += b_h;
		
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color);
		
		start_y = MAX_Y/2 - (3*b_h);
		end_y = start_y + b_h;
		
		LCD_DrawRectangle(start_x, start_y, end_x, end_y, color);
}

void Restore_Button_Meal_Color(uint16_t color1, uint16_t color2){
		LCD_DrawLine(5, MAX_Y-1, end_button1, MAX_Y-1, color1);
		LCD_DrawLine(5, MAX_Y, 5, (MAX_Y - (MAX_Y/5))+5, color1);
		LCD_DrawLine(end_button1, MAX_Y, end_button1, (MAX_Y - (MAX_Y/5))+5, color1);
		LCD_DrawLine(5, (MAX_Y - (MAX_Y/5))+5, end_button1, (MAX_Y - (MAX_Y/5))+5, color1);			
		GUI_Text(38, ((MAX_Y - (MAX_Y/5))+5) + 23, (uint8_t *) "MEAL", color2, White);
}


void Restore_Button_Snack_Color(uint16_t color1, uint16_t color2){
	LCD_DrawLine(end_button1 + 10, MAX_Y-1, MAX_X-5, MAX_Y-1, color1);
	LCD_DrawLine(end_button1 + 10, MAX_Y, end_button1 + 10, (MAX_Y - (MAX_Y/5))+5, color1);
	LCD_DrawLine(MAX_X-5, MAX_Y, MAX_X-5, (MAX_Y - (MAX_Y/5))+5, color1);
	LCD_DrawLine(end_button1 + 10, (MAX_Y - (MAX_Y/5))+5, MAX_X-5, (MAX_Y - (MAX_Y/5))+5, color1);
	GUI_Text((end_button1+10)+38,(((MAX_Y - (MAX_Y/5))+5))+23, (uint8_t *) "SNACK", color2, White);
}

void Dead(void){
		int i;
		disable_timer(0);
	//	disable_timer(1); mi serviva disabilitarlo quando gestiva il decrease battery
		disable_RIT();   
		Restore_Avatar();  /*It is necessary in order to have puppy in the start position*/
		Restore_Button_Meal_Color(White, Black);
		Restore_Button_Snack_Color(White, Black);
		dead = 1;
    sound = 1;
	  numSong = 3;
	
		/* set the start cordinates of the puppy */
		first_rectangle_coordinates();
	
		/*Delete */	
		Draw_Puppy(start_x, start_y, end_x, end_y, base_x, White, White);
	
		for(i=0; i<3; i++){
			
		  start_x -= 25;
			base_x -= 25;		
			end_x -= 25;
			
			/*Draw puppy*/
			Draw_Puppy(start_x, start_y, end_x, end_y, base_x, Black, Red);
			/*Delete */	
			Draw_Puppy(start_x, start_y, end_x, end_y, base_x, White, White);
		}
		
		End_Screen(Black);

		LCD_DrawRectangle(end_button1, (MAX_Y - (MAX_Y/5)) + 5 , end_button1 + 10, MAX_Y ,White);
	
		GUI_Text(38, ((MAX_Y - (MAX_Y/5))+5) + 23, (uint8_t *) "    ", White, White);
		GUI_Text((end_button1+10)+38,(((MAX_Y - (MAX_Y/5))+5))+23, (uint8_t *) "     ", White, White);
	
		GUI_Text((MAX_X/2) - 13, ((MAX_Y - (MAX_Y/5))+5) + 23, (uint8_t *) "RESET", Black, White); //(MAX_X/2)-13 = (MAX_X/2-10)-3: -10 because white space at the bottom of the screen (on the x-axis) begins 5 pixel to 0_X and it ends 5 pixel from MAX_X 
		enable_RIT();
}


void Decrease_Battery(void){
	
		grade_satiety --;
		grade_happiness --;
		LCD_DrawRectangle(((grade_happiness*level_x)+(happiness_x + 5))+2, (MAX_Y/7)+2, ((happiness_x + 5) + (grade_happiness*level_x) + level_x)-2, (MAX_Y/5)-2, White);	
		LCD_DrawRectangle(((grade_satiety*level_x)+(satiety_x + 5))+2, (MAX_Y/7)+2, ((satiety_x + 5) + (grade_satiety*level_x) + level_x)-2, (MAX_Y/5)-2, White);	
		
		if(grade_happiness ==0 || grade_satiety == 0)	Dead();
}



void Character_Happiness_Satiety(void){
	int  endx_batteryhapp, endx_batterysat;
	satiety_x = (end_button1+10)+30;
	
	endx_batteryhapp = ((happiness_x + 5) + (5*level_x));
	endx_batterysat = ((satiety_x + 5) + (5*level_x));

	GUI_Text(happiness_x, (MAX_Y/12)-3, (uint8_t *) "Happiness", Black, White);
	GUI_Text(satiety_x, (MAX_Y/12)-3, (uint8_t *) "Satiety", Black, White);
	
	LCD_DrawRectangle((happiness_x + 2), (MAX_Y/7)-3, endx_batteryhapp + 3, (MAX_Y/5)+3, Black);
  LCD_DrawRectangle(endx_batteryhapp+3, (MAX_Y/7)+3, endx_batteryhapp+7, (MAX_Y/5)-5, Black);
	
	LCD_DrawRectangle((satiety_x + 2), (MAX_Y/7)-3, endx_batterysat +3, (MAX_Y/5)+3, Black);
	LCD_DrawRectangle(endx_batterysat+3, (MAX_Y/7)+3, endx_batterysat+7, (MAX_Y/5)-5, Black);
	
	Charge_Satiety_Happiness();
	
}

void Buttons(void){
	
		LCD_DrawRectangle(0, MAX_Y - (MAX_Y/5), MAX_X, (MAX_Y - (MAX_Y/5)) + 5, Black);
		LCD_DrawRectangle(end_button1, (MAX_Y - (MAX_Y/5)) + 5 , end_button1 + 10, MAX_Y ,Black);
		LCD_DrawRectangle(0, MAX_Y - (MAX_Y/5), 5, MAX_Y, Black);
		LCD_DrawRectangle(MAX_X, MAX_Y - (MAX_Y/5), MAX_X -5, MAX_Y, Black);
	
		GUI_Text(38, ((MAX_Y - (MAX_Y/5))+5) + 23, (uint8_t *) "MEAL", Black, White);
		GUI_Text((end_button1+10)+38,(((MAX_Y - (MAX_Y/5))+5))+23, (uint8_t *) "SNACK", Black, White);
}


void Change_Animation(void){
		
		//eyes
		LCD_DrawRectangle(base_x_animation+12, start_y_animation-6, base_x_animation+18, end_y_animation-12, White);
		LCD_DrawRectangle(base_x_animation+26, start_y_animation-6, base_x_animation+32, end_y_animation-12, White);
		
		LCD_DrawRectangle(base_x_animation+14, start_y_animation-6, base_x_animation+20, end_y_animation-14, Black);
		LCD_DrawRectangle(base_x_animation+28, start_y_animation-6, base_x_animation+34, end_y_animation-14, Black);
	
		//hand
		LCD_DrawRectangle(start_x_animation-12, start_y_animation, start_x_animation-24, end_y_animation-9, White);
		LCD_DrawRectangle(start_x_animation-6, start_y_animation+6, start_x_animation-12, end_y_animation, White);
		LCD_DrawRectangle(start_x_animation-12, end_y_animation, start_x_animation-18, end_y_animation+6, White);
		LCD_DrawRectangle(start_x_animation-24, start_y_animation+6, start_x_animation-30, start_y_animation+12, White);

		LCD_DrawRectangle(start_x_animation-18, start_y_animation+6, start_x_animation-24, start_y_animation+12, Black);
		LCD_DrawRectangle(start_x_animation-6, end_y_animation, start_x_animation-12, end_y_animation+6, Black);
		LCD_DrawRectangle(start_x_animation-12, end_y_animation + 6, start_x_animation-24, end_y_animation+12, Black);
		LCD_DrawRectangle(start_x_animation-24, start_y_animation+12, start_x_animation-30, end_y_animation+6, Black);
	
}

void Restore_Avatar(void){
	
		//eyes
		LCD_DrawRectangle(base_x_animation+14, start_y_animation-6, base_x_animation+20, end_y_animation-14, White);
		LCD_DrawRectangle(base_x_animation+28, start_y_animation-6, base_x_animation+34, end_y_animation-14, White);
	
		LCD_DrawRectangle(base_x_animation+12, start_y_animation-6, base_x_animation+18, end_y_animation-12, Black);
		LCD_DrawRectangle(base_x_animation+26, start_y_animation-6, base_x_animation+32, end_y_animation-12, Black);
		
		//hand
		LCD_DrawRectangle(start_x_animation-18, start_y_animation+6, start_x_animation-24, start_y_animation+12, White);
		LCD_DrawRectangle(start_x_animation-6, end_y_animation, start_x_animation-12, end_y_animation+6, White);
		LCD_DrawRectangle(start_x_animation-12, end_y_animation + 6, start_x_animation-24, end_y_animation+12, White);
		LCD_DrawRectangle(start_x_animation-24, start_y_animation+12, start_x_animation-30, end_y_animation+6, White);
		
		LCD_DrawRectangle(start_x_animation-12, start_y_animation, start_x_animation-24, end_y_animation-9, Black);
		LCD_DrawRectangle(start_x_animation-6, start_y_animation+6, start_x_animation-12, end_y_animation, Black);
		LCD_DrawRectangle(start_x_animation-12, end_y_animation, start_x_animation-18, end_y_animation+6, Black);
		LCD_DrawRectangle(start_x_animation-24, start_y_animation+6, start_x_animation-30, start_y_animation+12, Black);
	
}

void Age(int h,int m, int s){
	char time_in_char[13] = "";
	int space;
	
	space = sprintf(time_in_char,"Age: %02d:%02d:%02d", h, m, s); //%02d means an integer, left padded with zeros up to 2 digits.
	GUI_Text((MAX_X/2 - (space/2)*8), 6, (uint8_t *) time_in_char, Black, White); // space/2 * 8 pixel
}


void Start_Screen(void){
	
		first_rectangle_coordinates();
		color1 = Black;
		color2 = Red;
	
		Draw_Puppy(start_x, start_y, end_x, end_y, base_x, color1, color2);
		
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
