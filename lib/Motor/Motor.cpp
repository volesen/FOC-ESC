#define PWM_TICKS 1024

#include "Motor.hpp"


///====================================================================================
///motor_pwm definition
///====================================================================================
motor_pwm::motor_pwm(const float32_t &A, const float32_t &B, const float32_t &C)
    : A(A), B(B), C(C) { }

//Returns the largest pwm phase magnitude
float32_t motor_pwm::get_max() const
{
    float32_t max = A < 0 ? -A : A;

    //If B is larger than C then check if magnitude of B is larger than max
    if (B > C && ((B < 0 ? -B : B) > max))
        max = B;
    //Else check if magnitude of C is larger than max
    else if ((C < 0 ? -C : C) > max)
        max = C;

    return max;
}

motor_pwm motor_pwm::get_scaled(const float32_t &bound_max) const 
{
    //TODO: REDO so that it returns value between 0-100 (set_duty is given percent)
    static float32_t range_shifter = PWM_TICKS / 2;
    float32_t scaler = PWM_TICKS / bound_max / 2;

    return motor_pwm(A * scaler + range_shifter, 
                     B * scaler + range_shifter, 
                     C * scaler + range_shifter);
}


///====================================================================================
///Motor definition
///====================================================================================
bool Motor::_initialized = false;

motor_pwm Motor::get_pwm()
{
    // if (!_initialized)
    //     throw 0;
    // else
        return _pwm;
}

void Motor::set_pwm(const motor_pwm &pwm)
{
    // if (!_initialized)
    //     throw 0;
    // else
    // {
        float32_t max = pwm.get_max();

        if (max > _pwm_max_bound)
            _pwm_max_bound = max;

        _pwm = pwm.get_scaled(_pwm_max_bound);
    // }

    update_pwm();
}



bool Motor::_initialized = false;

motor_pwm Motor::get_pwm()
{
    // if (!_initialized)
    //     throw 0;
    // else
        return _pwm;
}

void Motor::set_pwm(const motor_pwm &pwm)
{
    // if (!_initialized)
    //     throw 0;
    // else
    // {
        float32_t max = pwm.get_max();

        if (max > _pwm_max_bound)
            _pwm_max_bound = max;

        _pwm = pwm.get_scaled(_pwm_max_bound);
    // }

    update_pwm();
}

///====================================================================================
///Motor0 definition
///====================================================================================

void Motor0::update_pwm()
{
    
}

Motor0& Motor0::get_motor()
{
    static Motor0 motor;

    return motor;
}

void Motor0::initialize()
{
    //TODO:USE MCPWM INSTEAD

    //Check if already initialized
    // if (_initialized)
    //     throw 0;

    //Default values for Motor0 phases (pins B6, B7, B4)





    //TODO: change all TO 0






    _pwm = motor_pwm(511, 255, 127); //Initialize to no duty cycle (0 value)

    

    //Set initialized state so that methods can be used
    _initialized = true;
}


// ///====================================================================================
// ///Motor1 definition
// ///====================================================================================

// void Motor1::update_pwm()
// {
//     PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, (uint16_t)_pwm.A);
//     PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, (uint16_t)_pwm.B);
//     PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, (uint16_t)_pwm.C);
// }

// Motor1& Motor1::get_motor()
// {
//     static Motor1 motor;

//     return motor;
// }

// void Motor1::initialize()
// {
//     //Default values for Motor1 phases (pins B5, E4, E5)
//     _pwm = motor_pwm(); //Initialize to no duty cycle (0 value)

//     //Check if already initialized
//     if (_initialized)
//         throw 0;

//     //Set the PWM clock to be the system clock divided by 4
//     SysCtlPWMClockSet(SYSCTL_PWMDIV_1); 
    
//     //Enable PWM module 0 if not enabled
//     if (!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))             
//     {
//         SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);             //Enable PWM module 0
//         while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)) {}    //Wait for PWM module 0 to be ready
//     }
    
//     //Enable port B and E if not enabled
//     if (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
//     {
//         SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);            //Enable port B
//         SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);            //Enable port E

//         //Wait for ports to become enabled
//         while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)) {}   //Wait for GPIOB to be ready
//         while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}   //Wait for GPIOE to be ready
//     }

//     //Set pins to PWM mode
//     GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
//     GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
//     GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);

//     //Route PWM output to correct pins (some outputs go to multiple pins)
//     GPIOPinConfigure(GPIO_PB5_M0PWM3);
//     GPIOPinConfigure(GPIO_PE4_M0PWM4);
//     GPIOPinConfigure(GPIO_PE5_M0PWM5);

//     //Configure PWM generator 1 and 2 in module 0 to count down and to update outputs asynchronously (fastest response time)
//     PWMGenConfigure(PWM0_BASE, PWM_GEN_1, 
//                     PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
//     PWMGenConfigure(PWM0_BASE, PWM_GEN_2, 
//                     PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

//     //Set the period to 1024 ticks (40MHz (System clock) / 1 (PWM divider) / (1024 + 1) = 39024,3902... Hz)
//     PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, PWM_TICKS);
//     PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, PWM_TICKS);
    
//     //Set pulse widths to default values
//     update_pwm();

//     //Enable PWM generator 1 and 2 in module 0
//     PWMGenEnable(PWM0_BASE, PWM_GEN_1);
//     PWMGenEnable(PWM0_BASE, PWM_GEN_2);

//     //Enable output on selected pins
//     PWMOutputState(PWM0_BASE, (PWM_OUT_3_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT), true);

//     //Set initialized state so that methods can be used
//     _initialized = true;
// }
