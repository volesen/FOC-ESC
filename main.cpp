#define __FPU_PRESENT 1U
#define ARM_MATH_CM4
#include <arm_math.h>

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

#include "driverlib/adc.h"
#include "driverlib/sysctl.h" 
#include "driverlib/gpio.h" 
#include "driverlib/debug.h" 
#include "driverlib/pwm.h" 
#include "driverlib/pin_map.h" 
#include "driverlib/rom.h" 

#include "TM4C123GH6PM.h"

int32_t result;

int main()
{
	uint32_t ui32Value;
	
// Enable the ADC0 module.
	
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	
// Wait for the ADC0 module to be ready. 
	
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) { }

// Enable the first sample sequencer to capture the value of channel 0 when the processor trigger occurs. 

ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_ALWAYS, 0); 
ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0); 
ADCSequenceEnable(ADC0_BASE, 0);

/*
// Trigger the sample sequence. 

ADCProcessorTrigger(ADC0_BASE, 0);
*/
// Wait until the sample sequence has completed.

while(!ADCIntStatus(ADC0_BASE, 0, false)) { }

// Read the value from the ADC.

ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
	
	int32_t i = 0;
	while(1) 
	{

	}
}


/*
volatile unsigned long delay;

float32_t ADC_Resolution = 0.0008;
float32_t M0Current = 0;
float32_t M1Current = 0;

void ADC_Init(){
  //Initialize port PE1 and PE2 for ADC input
  SYSCTL_RCGC2_R |= 0x10;               // Enable clock for Port E
  delay = SYSCTL_RCGC2_R;               // Clock stabilization time
  GPIO_PORTE_DIR_R &= ~0x2;             // Define PE1 and PE2 as inputs
  GPIO_PORTE_AFSEL_R |= 0x2;            // Set PE1 and PE2 to "alternate function" aka. Analog
  GPIO_PORTE_DEN_R &= ~0x2;             // Disable digital on PE1 and PE2
  GPIO_PORTE_AMSEL_R |= 0x2;            // Enable analog functions on PE1 and PE2
  SYSCTL_RCGC0_R |= 0x30000;            // Enable ADC 1 and 2
  delay = SYSCTL_RCGC2_R;
  
  //ADC0 Setup using pin PE1        
  SYSCTL_RCGC0_R &= ~0x300;             // Set sample sequencer for 125k samples a sec
  ADC0_SSPRI_R = 0x123;                 // Set sequencer 3 for highest priority
  ADC0_ACTSS_R &= ~0x8;                 // Disable sample sequencer 3 during setup
  ADC0_EMUX_R = (0xF<<12);              // Set sequencer 3 for always on
  ADC0_SSMUX3_R &= ~0xF;                // Clear SS3 bits
  ADC0_SSMUX3_R += 2;                   // Set input to "Ain2" (PE1)
  ADC0_SSCTL3_R = 0x6;                  // Set sample 1 as end bit, and enable interrupts
  ADC0_ACTSS_R |= 0x8;                  // Enable sample sequencer 3 again
  ADC0_PSSI_R |= (1<<3);                // Set SS3 bit
  ADC0_PSSI_R |= (1<<27);               // Set SYNCWAIT bit for synchronized ADC's
  
  //ADC1 Setup using pin PE2
  SYSCTL_RCGC0_R &= ~0xc00;             // Set sample sequencer for 125k samples a sec
  ADC1_SSPRI_R = 0x123;                 // Set sequencer 3 for highest priority
  ADC1_ACTSS_R &= ~0x8;                 // Disable sample sequencer 3 during setup
  ADC1_EMUX_R = (0xF<<12);              // Set sequencer 3 for always on
  ADC1_SSMUX3_R &= ~0xF;                // Clear SS3 bits
  ADC1_SSMUX3_R += 1;                   // Set input to "Ain1" (PE2)
  ADC1_SSCTL3_R = 0x6;                  // Set sample 1 as end bit, and enable interrupts
  ADC1_ACTSS_R |= 0x8;                  // Enable sample sequencer 3 again
  ADC1_PSSI_R |= (1<<3);                // Set SS3 bit
  ADC1_PSSI_R |= (1<<27);               // Set SYNCWAIT bit for synchronized ADC's

  //Set ADC's to run synchronous
  ADC0_PSSI_R |= (1<<31);               //ADC0 GSYNC run
  ADC1_PSSI_R |= (1<<31);               //ADC1 GSYNC run
}

int main(){
  //Initialize ADC's
  ADC_Init();

  //Read data from ADC0 and convert to Voltage
  if(ADC0_RIS_R&0x08==1){
    M0Current = ADC0_SSFIFO3_R&0xFFF * ADC_Resolution;
    ADC0_ISC_R = 0x0008;
  }

  //Read data from ADC1 and convert to Voltage
  if(ADC1_RIS_R&0x08==1){
    M1Current = ADC1_SSFIFO3_R&0xFFF * ADC_Resolution;
    ADC1_ISC_R = 0x0008;
  }
}
*/
