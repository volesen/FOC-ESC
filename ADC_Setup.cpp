#include <tm4c123gh6pm.h>
volatile unsigned long delay;

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
  


  //ADC1 Setup using pin PE2
  SYSCTL_RCGC0_R &= ~0xc00;             // Set sample sequencer for 125k samples a sec
  ADC1_SSPRI_R = 0x123;                 // Set sequencer 3 for highest priority
  ADC1_ACTSS_R &= ~0x8;                 // Disable sample sequencer 3 during setup
  ADC1_EMUX_R = (0xF<<12);              // Set sequencer 3 for always on
  ADC1_SSMUX3_R &= ~0xF;                // Clear SS3 bits
  ADC1_SSMUX3_R += 1;                   // Set input to "Ain1" (PE2)
  ADC1_SSCTL3_R = 0x6;                  // Set sample 1 as end bit, and enable interrupts
  ADC1_ACTSS_R |= 0x8;                  // Enable sample sequencer 3 again
}

int main(){
ADC_Init();
}