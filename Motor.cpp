#define ARM_MATH_CM4
#define __FPU_PRESENT 1U
#include <arm_math.h>
#include <stdint.h>

#include "Motor.hpp"

//Scales to a 16-bit value
motor_outputs motor_outputs::get_scaled() const
{
    //Find maximum output
    

    //outputs.A =

    return 

}


void Motor1::initialize()
{
    	//Set up clocks
        
        //I don't think we should be doing the first line here
        //Might become an unexpected side effect
		SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); 
		SysCtlPWMClockSet(SYSCTL_PWMDIV_64); 
		
        //Enable PWM module 0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)) {}    //Wait for PWM module 0 to be ready
		
        //Enable port B
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        //Set pins to PWM
		GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
		GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);

        GPIOPinConfigure(GPIO_PB6_M0PWM0);
		GPIOPinConfigure(GPIO_PB7_M0PWM1);

        //Configure PWM generator 0 in module 0 to count down and to update outputs asynchronously
		PWMGenConfigure(PWM0_BASE, PWM_GEN_0, 
                        PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

        //Set the period to 400 ticks (how many hz?)
		PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 400);
		
        
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 200);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 100);

        //Enable PWM generator 0 in module 0
		PWMGenEnable(PWM0_BASE, PWM_GEN_0);

        //Enable output on selected pins
		PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT), true);
}

motor_outputs Motor1::get_outputs()
{
    return _outputs;
}

void Motor1::set_pwm(const motor_outputs &outputs)
{
    _outputs = outputs;

    
}

void Motor1::update_pwm(const motor_outputs &outputs)
{
    set_pwm(outputs.get_scaled());
}