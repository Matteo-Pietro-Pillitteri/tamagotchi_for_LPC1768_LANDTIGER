#include <string.h>
#include <stdint.h>

/* lib_adc.c */
void ADC_init (void);
void ADC_start_conversion (void);
float Return_Scaling(void);

/* IRQ_adc.c */
void ADC_IRQHandler(void);
