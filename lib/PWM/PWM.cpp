#define MCPWM_BASE_CLK (2 * APB_CLK_FREQ)                       //2*APB_CLK_FREQ = 160Mhz
#define MCPWM_CLK_PRESCL 0                                      //MCPWM clock prescale 
#define TIMER_CLK_PRESCALE 0                                    //Timer submodule prescale (=160 MHz)
#define PWM_FREQUENCY (16*1000)                                 //PWM frequency (=16KHz with resolution of (160MHz/16KHz=10000) at timer = 160MHz)

#define MCPWM_FREQUENCY (MCPWM_BASE_CLK/(MCPWM_CLK_PRESCL +1))      //Final frequency of MCPWM module
#define TIMER_FREQUENCY (MCPWM_FREQUENCY/(TIMER_CLK_PRESCALE + 1))  //Final timer frequency
#define PWM_PERIOD (TIMER_FREQUENCY/PWM_FREQUENCY)                  //PWM period length in timer ticks

#define PWM0_pin_A 32
#define PWM0_pin_B 33
#define PWM0_pin_C 25

#define PWM1_pin_C 27
#define PWM1_pin_B 14
#define PWM1_pin_A 12


#include <Arduino.h>

//#include <driver/mcpwm.h>
//#include <soc/soc.h>
//#include <soc/mcpwm_reg.h>
#include <driver/periph_ctrl.h>
#include <soc/mcpwm_struct.h>

#include "PWM.hpp"

//TODO: Implement error handling


///====================================================================================
///pwm_phases definition
///====================================================================================
pwm_phases::pwm_phases(const float& A, const float& B, const float& C)
    : A(A), B(B), C(C) { }

//Returns the largest pwm phase magnitude
float pwm_phases::get_max() const
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

pwm_phases pwm_phases::create_corrected(const float &bound_max) const 
{
    if (bound_max == 0)
        return pwm_phases(PWM_PERIOD / 2, PWM_PERIOD / 2, PWM_PERIOD / 2);
    else
    {
        float scaler = PWM_PERIOD / bound_max / 2;      //Scales value to be in [-PWM_PERIOD/2; PWM_PERIOD/2]
        static float range_shifter = PWM_PERIOD / 2;    //Shifts value to be in [0; PWM_PERIOD]

        return pwm_phases(A * scaler + range_shifter,    //Returns value in [0; PWM_PERIOD]
                        B * scaler + range_shifter, 
                        C * scaler + range_shifter);
    }
}

pwm_phases pwm_phases::create_decorrected(const float &bound_max) const 
{
    if (bound_max == 0)
        return pwm_phases(0, 0, 0);
    else
    {
        float scaler = PWM_PERIOD / bound_max / 2;      //Scales value to be in [-PWM_PERIOD/2; PWM_PERIOD/2]
        static float range_shifter = PWM_PERIOD / 2;    //Shifts value to be in [0; PWM_PERIOD]

        return pwm_phases((A - range_shifter) / scaler,  //Returns value in [-bound_max; bound_max]
                         (B - range_shifter) / scaler,
                         (C - range_shifter) / scaler);
    }
}

///====================================================================================
///PWM definition
///====================================================================================
PWM& PWM::get(motor_id motor)
{
    switch (motor)
    {
        case motor0:
            return PWM0::get();
        case motor1:
            return PWM1::get();
        default:
            //Throw an error or something
            break;
    }
}

float PWM::get_phases_max_bound() const
{
    // if (!_initialized)
    //     throw 0;
    // else
        return _phases_max_bound;
}

pwm_phases PWM::get_phases(bool return_decorrected) const
{
    // if (!_initialized)
    //     throw 0;
    // else
    if (return_decorrected)
        return _phases.create_decorrected(_phases_max_bound);
    else
        return _phases;
}

void PWM::set_phases(const pwm_phases &pwm)
{
    // if (!_initialized)
    //     throw 0;
    // else
    // {
        float max = pwm.get_max();

        if (max > _phases_max_bound)
            _phases_max_bound = max;

        _phases = pwm.create_corrected(_phases_max_bound);
        
        update_phases();
    // }

}

void PWM::set_phases(const float &A, const float &B, const float &C)
{
    //This is the overloaded function even though it is inefficient to create a new struct
    //This is because set_phases(pwm) changes _phases in one assignment.
    //If set_phases(A, B, C) was the "main" function then _phases would have been assigned to four times with the straight forward approach.
    //This is bad if an interrupt comes in the middle and tries to get _phases.
    //The _phases value will potentially not have been scaled yet.
    set_phases(pwm_phases(A, B, C));
}

void PWM::set_phases_low(bool A, bool B, bool C)
{
    // if (!_initialized)
    //     throw 0;
    // else
    // {
        if (A)
            _phases.A = 0;
        if (B)
            _phases.B = 0;
        if (C)
            _phases.C = 0;

        update_phases();
        // }
}

void PWM::set_phases_high(bool A, bool B, bool C)
{
    // if (!_initialized)
    //     throw 0;
    // else
    // {
        if (A)
            _phases.A = PWM_PERIOD;
        if (B)
            _phases.B = PWM_PERIOD;
        if (C)
            _phases.C = PWM_PERIOD;

        update_phases();
    // }
}

void PWM::initialize_all(float initial_phases_max_bound)
{
    for (uint8_t motor = 0; motor < NUM_MOTORS; motor++)
        get((motor_id)motor).initialize(initial_phases_max_bound);
}

///====================================================================================
///PWM0 definition
///====================================================================================
bool PWM0::_initialized = false;

PWM0::PWM0()
    : _pins{PWM0_pin_A, PWM0_pin_B, PWM0_pin_C}
{
    _phases = pwm_phases(0, 0, 0);
}

PWM0& PWM0::get()
{
    //Is instantiated on first call. 
    //Subsequent calls simply return the already existing pwm
    static PWM0 pwm;

    if (!pwm._initialized)
        pwm.initialize();

    return pwm;
}

const uint8_t *PWM0::get_pins() const
{
    // if (!_initialized)
    //     throw 0;
    // else
        return _pins;
}

bool PWM0::get_initialized() const
{
    return PWM0::_initialized;
}

PWM0& PWM0::initialize(float initial_phases_max_bound)
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
        gpio_set_direction((gpio_num_t)_pins[i], GPIO_MODE_OUTPUT);
        
        //Route PWM operator output 0A, 1A, 2A to their respective pins via the GPIO matrix. 
        //PWM0_OUT0A_IDX is the index for operator 0A in MCPWM module 0
        //PWM0_OUT0A_IDX +1 is PWM0_OUT0B_IDX, +2 is PWM0_OUT1A_IDX
        //The two last parameters are output signal inversion and output control/enable inversion.
        //The signal is not inverted.
        //Output control/enable inversion is disabled as it causes things to not turn off completely.
        gpio_matrix_out(_pins[i], PWM0_OUT0A_IDX + i*2, 0, 0);
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

    //Set initial max pwm bound
    _phases_max_bound = initial_phases_max_bound;

    //Return pwm so that settings can be set up immediately after with easy syntax
    return get();
}

void PWM0::update_phases()
{
    MCPWM0.channel[0].cmpr_value[0].cmpr_val = (uint32_t)_phases.A;       //Set duty period of operator 0 (tied to channel 0) output A to _phases.A
    MCPWM0.channel[1].cmpr_value[0].cmpr_val = (uint32_t)_phases.B;       //Set duty period of operator 1 (tied to channel 1) output A to _phases.B
    MCPWM0.channel[2].cmpr_value[0].cmpr_val = (uint32_t)_phases.C;       //Set duty period of operator 2 (tied to channel 2) output A to _phases.C
}


// ///====================================================================================
// ///PWM1 definition
// ///====================================================================================
bool PWM1::_initialized = false;

PWM1::PWM1()
    : _pins{PWM1_pin_A, PWM1_pin_B, PWM1_pin_C}
{
    _phases = pwm_phases(0, 0, 0);
}

PWM1& PWM1::get()
{
    //Is instantiated on first call. 
    //Subsequent calls simply return the already existing pwm
    static PWM1 pwm;

    if (!pwm._initialized)
        pwm.initialize();        

    return pwm;
}

const uint8_t *PWM1::get_pins() const
{
    // if (!_initialized)
    //     throw 0;
    // else
        return _pins;
}

bool PWM1::get_initialized() const
{
    return PWM1::_initialized;
}

PWM1& PWM1::initialize(float initial_phases_max_bound)
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
        gpio_set_direction((gpio_num_t)_pins[i], GPIO_MODE_OUTPUT);
        
        //Route PWM operator output 0B, 1B, 2B to their respective pins via the GPIO matrix. 
        //PWM0_OUT0A_IDX is the index for operator 0A in MCPWM module 0
        //PWM0_OUT0A_IDX, +1 is PWM0_OUT0B_IDX, +2 is PWM0_OUT1A_IDX
        //The two last parameters are output signal inversion and output control/enable inversion.
        //The signal is not inverted.
        //Output control/enable inversion is disabled as it causes things to not turn off completely.
        gpio_matrix_out(_pins[i], PWM0_OUT0A_IDX + i*2 + 1, 0, 0);
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

    //Set initial max pwm bound
    _phases_max_bound = initial_phases_max_bound;

    //Return pwm so that settings can be set up immediately after with easy syntax
    return get();
}

void PWM1::update_phases()
{
    MCPWM0.channel[0].cmpr_value[1].cmpr_val = (uint32_t)_phases.A;       //Set duty period of operator 0 (tied to channel 0) output B to _phases.A
    MCPWM0.channel[1].cmpr_value[1].cmpr_val = (uint32_t)_phases.B;       //Set duty period of operator 1 (tied to channel 1) output B to _phases.B
    MCPWM0.channel[2].cmpr_value[1].cmpr_val = (uint32_t)_phases.C;       //Set duty period of operator 2 (tied to channel 2) output B to _phases.C
}