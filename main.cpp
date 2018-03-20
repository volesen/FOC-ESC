#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

#include "driverlib/sysctl.h" 
#include "driverlib/gpio.h" 
#include "driverlib/debug.h" 
#include "driverlib/pwm.h" 
#include "driverlib/pin_map.h" 
#include "driverlib/rom.h" 

#include "TM4C123GH6PM.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

int
main(void)
{
		//PWM
		SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); 
		SysCtlPWMClockSet(SYSCTL_PWMDIV_64); 
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)) {}
			
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
		GPIOPinConfigure(GPIO_PB6_M0PWM0);
		GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
		GPIOPinConfigure(GPIO_PB7_M0PWM1);
			
		PWMGenConfigure(PWM0_BASE, PWM_GEN_0, 
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
		PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 400);
			
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 200);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 100);

		PWMGenEnable(PWM0_BASE, PWM_GEN_0);
		PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT), true);
		

    while(1)
    {
				
    }
}