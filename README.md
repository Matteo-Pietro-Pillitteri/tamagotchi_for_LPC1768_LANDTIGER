# tamagotchi_for_LPC1768_LANDTIGER

-	Gameplay: by touching the avatar in the middle of the screen it is possible to see an animation of 2 seconds. In particular, the animation consists of a heart sliding up to the happines bar. By the way, the “cuddles animation” works only if the gamer touches the area covered by the avatar. When the animation ends the happines level is increased by one bar (value).  

-	Implementation: the animation is triggered when the point touched by the user is in a specific range. It is possible to get the coordinates of the selected point on the display using the getDisplayPoint function. This function is implemented in TouchPanel.c that is a file in the TouchPanel library. The function is invoked in the Rit Handler.  The Repetitive Interrupt Controller is set in the sample.c file using a value of 50ms (experimental value) to have a correct behaviour of the system. The TouchPanel library also contains a Display matrix. Every time that the getDisplayPoint returns a values, it is checked if display.x and display.y are allowed values. 
A couple of coordinates retrived by the touch are allowed if and only if: 
1) Display.y is greater than start_y and display.y is less than MAX_Y-122. 
2)	display.x is greater than start_ x_puppy and display.x is less than end_x_puppy. 
The avatar is displayed from the “start_x_puppy” to “end_x_puppy” on the x-Axis and from “start_x_puppy” to MAX_Y – 122 on the Y-Axis. 
If the allowed coordinates are allowed the Timer3 is enabled. It is initialized in the sample.c. It uses 4 match registers (MR0, MR1, MR2, MR3). 
When a match register interrupts a heart in the right position is displayed using the function Draw_Heart that draw the figure thanks to LCD_SetPoint function. The GLCD.c file implements the Draw_Heart function. 
The latter match register allows to increase the happines level using Increase_Satiety_Or_Happiness (simple function used also in the fist extrapoint).
-	Technical details:
 Library retrived by the course materials and usefull for the cuddles animation: 

1)	06_sample_TIMER_2: it allows an advanced management of times (optional parametres like match register, use of prescaler and match register configuration)
2)	12_sample_GLCD_TP: it contains the TouchPanel library that is necessary for the initialization and calibration of the touch screen provided by the board. It contains the function TP_Init() and TouchPanel_Calibrate().
3)	Any library that contains the RIT folder with its files (IRQ_RIT.c, lib_RIT.c, RIT.h) like the 08_sample_JOYSTICK library.

-	Usefull manual of the board used (LandTiger):
1) [LandTiger V2.0 LPC17XX - Development Board – user manual](https://os.mbed.com/media/uploads/wim/landtiger_v2.0_-_manual__v1.1.pdf)
2) [TouchScreen](https://www.ti.com/lit/an/slyt277/slyt277.pdf?ts=1674589208164&ref_url=https%253A%252F%252Fwww.google.com%252F)
