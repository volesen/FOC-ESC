#include <Arduino.h>

#include "ESC.hpp"

#include "Transform.hpp"
#include "PWM.hpp"
#include "QEncoder.hpp"
#include "ESC_Serial.hpp"

#define CLOSED_LOOP_MODE 1

#define VIRTUAL_POSITION_RESET_TIME_MS 80

#define PID_TORQUE_P                0.62
#define PID_TORQUE_I                0.47
#define PID_TORQUE_D                0
#define PID_TORQUE_I_MAX_CHANGE     0.4
#define PID_TORQUE_D_WINDOW_FAST    1
#define PID_TORQUE_D_WINDOW_SLOW    2

#define PID_WASTE_P                 0.3
#define PID_WASTE_I                 0.2
#define PID_WASTE_D                 0
#define PID_WASTE_I_MAX_CHANGE      0.5
#define PID_WASTE_D_WINDOW_FAST     1
#define PID_WASTE_D_WINDOW_SLOW     2


#pragma region Initialization
ESC::ESC()
{
    //Initailize classes to interact with hardware
    initialize_classes();

    //Align rotors so that virtual position is known
    for (int i = 0; i < NUM_MOTORS; i++)
        reset_rotor_virtual_position((motor_id)i);
}

void ESC::initialize_classes()
{
    //Initialize serial interface
    ESC_Serial::initialize();

    //Initialize PID controllers
    pid_torque = PID_Controller(PID_TORQUE_P, 
                                PID_TORQUE_I, 
                                PID_TORQUE_D, 
                                PID_TORQUE_I_MAX_CHANGE,
                                PID_TORQUE_D_WINDOW_FAST,
                                PID_TORQUE_D_WINDOW_SLOW);
    pid_waste =  PID_Controller(PID_WASTE_P, 
                                PID_WASTE_I, 
                                PID_WASTE_D, 
                                PID_WASTE_I_MAX_CHANGE,
                                PID_WASTE_D_WINDOW_FAST,
                                PID_WASTE_D_WINDOW_SLOW);

    //Initialize quadrature encoders
    QEncoder::initialize_all();

    //Initialize PWM module
    PWM::initialize_all();
}

void reset_rotor_virtual_position(motor_id motor)
{
    //Store pwm_phases
    pwm_phases previous_phases = PWM::get(motor).get_phases();

    //Force rotor into known position
    PWM::get(motor).set_phases_high(true, false, false);
    PWM::get(motor).set_phases_low(false, true, true);

    //Wait for rotor to physically rotate
    vTaskDelay(VIRTUAL_POSITION_RESET_TIME_MS / portTICK_PERIOD_MS);

    //Reset virtual position field in QEncoder class
    QEncoder::get(motor).reset_virtual_position();

    //Restore pwm_phases
    PWM::get(motor).set_phases(previous_phases);
}

#pragma endregion

ESC& ESC::get() { static ESC instance{}; return instance; }
void ESC::initialize() { get(); }

bool ESC::update()
{
    for (uint8_t motor = 0; motor < NUM_MOTORS; motor++)
    {
        Iabc phases {0, 0, 0};

        ADC_Motor::get((motor_id)motor)
    }
}