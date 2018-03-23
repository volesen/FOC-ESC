#define ARM_MATH_CM4

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

//#include <driverlib/fpu.h>
#include "driverlib/sysctl.h" 
#include "driverlib/gpio.h" 
#include "driverlib/debug.h" 
#include "driverlib/pwm.h" 
#include "driverlib/pin_map.h" 
#include "driverlib/rom.h"

#include "TM4C123GH6PM.h"

#include "Motor.hpp"

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
	//Sets the MCU to run at 40MHz according to peripheral driver library
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

		motor_pwm pwm(1024, 512, 255);

		Motor0 m0 = Motor0();

		m0.initialize();
		m0.set_pwm(pwm);
		//Motor0::get_motor().initialize();
		//Motor0::get_motor().set_pwm(pwm);


    // //Set the PWM clock to be the system clock divided by 4
    // SysCtlPWMClockSet(SYSCTL_PWMDIV_1); 
    
    // //Enable PWM module 0 if not enabled
    // if (!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
    // {
    //     SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);             //Enable PWM module 0
    //     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)) {}    //Wait for PWM module 0 to be ready
    // }
    
    // //Enable port B if not enabled
    // if (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    // {
    //     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);            //Enable port B
    //     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)) {}   //Wait for GPIOB to be ready
    // }

    // //Set pins to PWM mode
    // GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    // GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
    // GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);

    // //Route PWM output to correct pins (some outputs go to multiple pins)
    // GPIOPinConfigure(GPIO_PB6_M0PWM0);
    // GPIOPinConfigure(GPIO_PB7_M0PWM1);
    // GPIOPinConfigure(GPIO_PB4_M0PWM2);

    // //Configure PWM generator 0 and 1 in module 0 to count down and to update outputs asynchronously (fastest response time)
    // PWMGenConfigure(PWM0_BASE, PWM_GEN_0, 
    //                 PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    // PWMGenConfigure(PWM0_BASE, PWM_GEN_1, 
    //                 PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    // //Set the period to 1024 ticks (40MHz (System clock) / 1 (PWM divider) / (1024 + 1) = 39024,3902... Hz)
    // PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 1024);
    // PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 1024);

    // //Set pulse widths to default values
    // PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1024);
    // PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 512);
    // PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 255);

    // //Enable PWM generator 0 and 1 in module 0
    // PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    // PWMGenEnable(PWM0_BASE, PWM_GEN_1);

    // //Enable output on selected pins
    // PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT), true);
		

    while(1)
    {
				
    }
}
