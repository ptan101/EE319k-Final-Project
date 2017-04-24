// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 04/10/2017 
// Student names: Zachary Chilton & Philip Tan
// Last modification date: 04/10/2017

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){
	uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x10;   		// Port E clock initialized
  delay = 0;        						  // allow time for clocks
	delay++;
	
  GPIO_PORTE_DIR_R &= ~0x04;	 		// PE2 now input
  GPIO_PORTE_AFSEL_R |= 0x04;     // Ain1 set on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // digital I/O cleared on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // analog function enabled on PE2
	
	SYSCTL_RCGCADC_R |= 0x01;				// ADC0 clock initialized
  delay = SYSCTL_RCGCADC_R;				// extra time to stabilize
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	
  ADC0_PC_R = 0x01; 							// 125 KHz sampling rate set
  ADC0_SSPRI_R = 0x0123;          // sq3 now highest priority
  ADC0_ACTSS_R &= ~0x0008;        // sq3 disabled during setup
  ADC0_EMUX_R &= ~0xF000;         // sq3 is software triggered
  ADC0_SSMUX3_R &= ~0x000F;       // SS3 channels cleared
  ADC0_SSMUX3_R += 1;             // Ain1 (PE2) set
  ADC0_SSCTL3_R = 0x0006;         // not measuring temperature, set END to prevent unintended behavior
	ADC0_IM_R &= ~0x0008;           // sq3 interrupts disabled
  ADC0_ACTSS_R |= 0x0008;       	// sq3 re-enabled
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	ADC0_PSSI_R = 0x08;							// initiate conversion
	
	while((ADC0_RIS_R&0x08) == 0){}	// while busy converting, wait
	ADC0_ISC_R = 0x08;							// done converting, indicate so
	
  return ADC0_SSFIFO3_R&0xFFF; 		// return conversion result
}
