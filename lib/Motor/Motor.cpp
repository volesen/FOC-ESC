#define MCPWM_BASE_CLK (2 * APB_CLK_FREQ)                       //2*APB_CLK_FREQ = 160Mhz
#define MCPWM_CLK_PRESCL 0                                      //MCPWM clock prescale 
#define TIMER_CLK_PRESCALE 0                                    //Timer submodule prescale (=160 MHz)
#define PWM_FREQUENCY (20*1000)                                 //PWM frequency (=20KHz with resolution of (160MHz/20KHz=8000) at timer = 160MHz)

#define MCPWM_FREQUENCY (MCPWM_BASE_CLK/(MCPWM_CLK_PRESCL +1))      //Final frequency of MCPWM module
#define TIMER_FREQUENCY (MCPWM_FREQUENCY/(TIMER_CLK_PRESCALE + 1))  //Final timer frequency
#define PWM_PERIOD (TIMER_FREQUENCY/PWM_FREQUENCY)                  //PWM period length in timer ticks


#include <Arduino.h>

//#include <driver/mcpwm.h>
//#include <soc/soc.h>
//#include <soc/mcpwm_reg.h>
#include <driver/periph_ctrl.h>
#include <soc/mcpwm_struct.h>

#include "Motor.hpp"

//TODO: Implement error handling


///====================================================================================
///motor_pwm definition
///====================================================================================
motor_pwm::motor_pwm(const float& A, const float& B, const float& C)
    : A(A), B(B), C(C) { }

//Returns the largest pwm phase magnitude
float motor_pwm::get_max() const
{
    float max = abs(A);

    //If B is larger than C then check if magnitude of B is larger than max
    if (B > C && (abs(B) > max))
        max = abs(B);
    //Else check if magnitude of C is larger than max
    else if (abs(C) > max)
        max = abs(C);

    return max;
}

motor_pwm motor_pwm::get_scaled(const float &bound_max) const 
{
    float scaler = PWM_PERIOD / bound_max / 2;      //Scales value to be in [-PWM_PERIOD/2; PWM_PERIOD/2]
    static float range_shifter = PWM_PERIOD / 2;    //Shifts value to be in [0; PWM_PERIOD]

    return motor_pwm(A * scaler + range_shifter, 
                     B * scaler + range_shifter, 
                     C * scaler + range_shifter);
}

motor_pwm motor_pwm::get_descaled(const float &bound_max) const 
{
    float scaler = PWM_PERIOD / bound_max / 2;      //Scales value to be in [-PWM_PERIOD/2; PWM_PERIOD/2]
    static float range_shifter = PWM_PERIOD / 2;    //Shifts value to be in [0; PWM_PERIOD]

    return motor_pwm((A - range_shifter) / scaler, 
                     (B - range_shifter) / scaler, 
                     (C - range_shifter) / scaler);
}

///====================================================================================
///Motor definition
///====================================================================================
Motor& Motor::get(char id)
{
    switch (id)
    {
        case 0:
            return Motor0::get();
        case 1:
            return Motor1::get();
        default:
            //Throw an error or something
            break;
    }
}

void Motor::initialize_all()
{
    Motor::get(0).initialize();
    Motor::get(1).initialize();
}

float Motor::get_pwm_max_bound() const
{
    // if (!_initialized)
    //     throw 0;
    // else
        return _pwm_max_bound;
}

motor_pwm Motor::get_pwm() const
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
        float max = pwm.get_max();

        if (max > _pwm_max_bound)
            _pwm_max_bound = max;

        _pwm = pwm.get_scaled(_pwm_max_bound);
        
        update_pwm();
    // }

}

void Motor::set_pwm(const float &A, const float &B, const float &C)
{
    //This is the overloaded function even though it is inefficient to create a new struct
    //This is because set_pwm(pwm) changes _pwm in one assignment.
    //If set_pwm(A, B, C) was the "main" function then _pwm would have been assigned to four times with the straight forward approach.
    //This is bad if an interrupt comes in the middle and tries to get _pwm.
    //The _pwm value will potentially not have been scaled yet.
    set_pwm(motor_pwm(A, B, C));
}

void Motor::set_pwm_low()
{
    // if (!_initialized)
    //     throw 0;
    // else
    // {
        _pwm.A = _pwm.B = _pwm.C = 0;

        update_pwm();
        // }
}

void Motor::set_pwm_high()
{
    // if (!_initialized)
    //     throw 0;
    // else
    // {
        _pwm.A = _pwm.B = _pwm.C = PWM_PERIOD;

        update_pwm();
    // }
}

///====================================================================================
///Motor0 definition
///====================================================================================
bool Motor0::_initialized = false;

Motor0::Motor0()
    : _pins{12, 14, 27}
{
    _pwm = motor_pwm(0, 0, 0);
}

Motor0& Motor0::get()
{
    //Is instantiated on first call. 
    //Subsequent calls simply return the already existing motor
    static Motor0 motor;        

    return motor;
}

const uint8_t *Motor0::get_pins() const
{
    // if (!_initialized)
    //     throw 0;
    // else
        return _pins;
}

bool Motor0::get_initialized() const
{
    return Motor0::_initialized;
}

Motor0& Motor0::initialize()
{
    //Check if already initialized
    // if (_initialized)
    //     throw 0;

    periph_module_enable(PERIPH_PWM0_MODULE);       //Enable MCPWM0 module
    
    //Pin initialization
    for (uint8_t i = 0; i < 3; i++)
    {
        //I do not understand exactly how this one works.
        //If it isn't here, behavior is inconsistent on SOME pins.
        //It apparently sets a register based on a mask but it does it in a weird way that I don't get.
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[(gpio_num_t)_pins[i]], PIN_FUNC_GPIO);

        //Set GPIO pin to output
        //Serial.println((gpio_num_t)_pins[i]);
        gpio_set_direction((gpio_num_t)_pins[i], GPIO_MODE_OUTPUT);
        
        //Route PWM operator output 0A, 1A, 2A to their respective pins via the GPIO matrix. 
        //PWM0_OUT0A_IDX is the index for operator 0A in MCPWM module 0
        //PWM0_OUT0A_IDX +1 is PWM0_OUT0B_IDX, +2 is PWM0_OUT1A_IDX
        //The two last parameters are output signal inversion and output control/enable inversion.
        //The signal is inverted because it seems inverted in the first place. 
        //Output control/enable inversion is disabled as it causes things to not turn off completely.
        gpio_matrix_out(_pins[i], PWM0_OUT0A_IDX + i*2, 1, 0);
    }    

    //Set up timer
    MCPWM0.clk_cfg.prescale = MCPWM_CLK_PRESCL;             //Set MCPWM base clock prescaler
    MCPWM0.timer[0].period.prescale = TIMER_CLK_PRESCALE;   //Set prescale of timer 0
    MCPWM0.timer[0].period.period = PWM_PERIOD;             //Set period of timer 0 which defines precision of PWM
    MCPWM0.timer[0].period.upmethod = 1;                    //Set update method to update each time timer 0 equals zero (TEZ) (avoids weird glitches)
    MCPWM0.timer[0].mode.mode = 1;                          //Set timer to count up

    //Set all PWM operators to use timer 0 as their reference
    MCPWM0.timer_sel.operator0_sel = 0;
    MCPWM0.timer_sel.operator1_sel = 0;
    MCPWM0.timer_sel.operator2_sel = 0;

    //Set duty period of operators to 0
    for (uint8_t i = 0; i < 3; i++)
    {   
        MCPWM0.channel[i].cmpr_value[0].cmpr_val = 0;       //Set duty period of operator i (tied to channel i) output A to 0
        MCPWM0.channel[i].cmpr_cfg.a_upmethod = BIT(0);     //Set operator i output A to update compare value on event TEZ
        
        //These settings can be used to invert the signal
        MCPWM0.channel[i].generator[0].utep = 0;            //Set operator i output A to do nothing when event (counting up timer equals period) happens
        MCPWM0.channel[i].generator[0].utea = 1;            //Set operator i output A to go low when event (counting up timer equals A compare value) happens
        MCPWM0.channel[i].generator[0].utez = 2;            //Set operator i output A to go high when event (counting up timer equals zero) happens
    }

    MCPWM0.timer[0].mode.start = 2;     //Set timer to run freely without any event as a stop condition

    //Update registers
    MCPWM0.update_cfg.global_up_en = 1;     //Enable active registers in MCPWM module 0 to be updated by shadow registers
    MCPWM0.update_cfg.global_force_up = 1;  //Force new update of active registers in MCPWM module 0
    MCPWM0.update_cfg.global_force_up = 0;  //Stop forcing the above update

    //Set initialized state so that methods can be used
    _initialized = true;

    //Return motor so that settings can be set up immediately after with easy syntax
    return get();
}

void Motor0::update_pwm()
{
    MCPWM0.channel[0].cmpr_value[0].cmpr_val = (uint32_t)_pwm.A;       //Set duty period of operator 0 (tied to channel 0) output A to _pwm.A
    MCPWM0.channel[1].cmpr_value[0].cmpr_val = (uint32_t)_pwm.B;       //Set duty period of operator 1 (tied to channel 1) output A to _pwm.B
    MCPWM0.channel[2].cmpr_value[0].cmpr_val = (uint32_t)_pwm.C;       //Set duty period of operator 2 (tied to channel 2) output A to _pwm.C
}


// ///====================================================================================
// ///Motor1 definition
// ///====================================================================================
bool Motor1::_initialized = false;

Motor1::Motor1()
    : _pins{25, 33, 32}
{
    _pwm = motor_pwm(0, 0, 0);
}

Motor1& Motor1::get()
{
    //Is instantiated on first call. 
    //Subsequent calls simply return the already existing motor
    static Motor1 motor;        

    return motor;
}

const uint8_t *Motor1::get_pins() const
{
    // if (!_initialized)
    //     throw 0;
    // else
        return _pins;
}

bool Motor1::get_initialized() const
{
    return Motor1::_initialized;
}

Motor1& Motor1::initialize()
{
    //Check if already initialized
    // if (_initialized)
    //     throw 0;

    periph_module_enable(PERIPH_PWM0_MODULE);       //Enable MCPWM0 module
    
    //Pin initialization
    for (uint8_t i = 0; i < 3; i++)
    {
        //I do not understand exactly how this one works.
        //If it isn't here, behavior is inconsistent on SOME pins.
        //It apparently sets a register based on a mask but it does it in a weird way that I don't get.
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[(gpio_num_t)_pins[i]], PIN_FUNC_GPIO);

        //Set GPIO pin to output
        //Serial.println((gpio_num_t)_pins[i]);
        gpio_set_direction((gpio_num_t)_pins[i], GPIO_MODE_OUTPUT);
        
        //Route PWM operator output 0B, 1B, 2B to their respective pins via the GPIO matrix. 
        //PWM0_OUT0A_IDX is the index for operator 0A in MCPWM module 0
        //PWM0_OUT0A_IDX, +1 is PWM0_OUT0B_IDX, +2 is PWM0_OUT1A_IDX
        //The two last parameters are output signal inversion and output control/enable inversion.
        //The signal is inverted because it seems inverted in the first place. 
        //Output control/enable inversion is disabled as it causes things to not turn off completely.
        gpio_matrix_out(_pins[i], PWM0_OUT0A_IDX + i*2 + 1, 1, 0);
    }    

    //Set up timer
    MCPWM0.clk_cfg.prescale = MCPWM_CLK_PRESCL;             //Set MCPWM base clock prescaler
    MCPWM0.timer[0].period.prescale = TIMER_CLK_PRESCALE;   //Set prescale of timer 0
    MCPWM0.timer[0].period.period = PWM_PERIOD;             //Set period of timer 0 which defines precision of PWM
    MCPWM0.timer[0].period.upmethod = 1;                    //Set update method to update each time timer 0 equals zero (TEZ) (avoids weird glitches)
    MCPWM0.timer[0].mode.mode = 1;                          //Set timer to count up

    //Set all PWM operators to use timer 0 as their reference
    MCPWM0.timer_sel.operator0_sel = 0;
    MCPWM0.timer_sel.operator1_sel = 0;
    MCPWM0.timer_sel.operator2_sel = 0;

    //Set duty period of operators to 0
    for (uint8_t i = 0; i < 3; i++)
    {   
        MCPWM0.channel[i].cmpr_value[1].cmpr_val = 0;       //Set duty period of operator i (tied to channel i) output B to 0
        MCPWM0.channel[i].cmpr_cfg.b_upmethod = BIT(0);     //Set operator i output B to update compare value on event TEZ
        
        //These settings can be used to invert the signal
        MCPWM0.channel[i].generator[1].utep = 0;            //Set operator i output B to do nothing when event (counting up timer equals period) happens
        MCPWM0.channel[i].generator[1].uteb = 1;            //Set operator i output B to go low when event (counting up timer equals B compare value) happens
        MCPWM0.channel[i].generator[1].utez = 2;            //Set operator i output B to go high when event (counting up timer equals zero) happens
    }

    MCPWM0.timer[0].mode.start = 2;         //Set timer to run freely without any event as a stop condition

    //Update registers
    MCPWM0.update_cfg.global_up_en = 1;     //Enable active registers in MCPWM module 0 to be updated by shadow registers
    MCPWM0.update_cfg.global_force_up = 1;  //Force new update of active registers in MCPWM module 0
    MCPWM0.update_cfg.global_force_up = 0;  //Stop forcing the above update

    //Set initialized state so that methods can be used
    _initialized = true;

    //Return motor so that settings can be set up immediately after with easy syntax
    return get();
}

void Motor1::update_pwm()
{
    MCPWM0.channel[0].cmpr_value[1].cmpr_val = (uint32_t)_pwm.A;       //Set duty period of operator 0 (tied to channel 0) output B to _pwm.A
    MCPWM0.channel[1].cmpr_value[1].cmpr_val = (uint32_t)_pwm.B;       //Set duty period of operator 1 (tied to channel 1) output B to _pwm.B
    MCPWM0.channel[2].cmpr_value[1].cmpr_val = (uint32_t)_pwm.C;       //Set duty period of operator 2 (tied to channel 2) output B to _pwm.C
}