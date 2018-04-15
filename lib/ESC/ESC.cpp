#include <Arduino.h>

#include "ESC.hpp"

#include "Transform.hpp"
#include "PWM.hpp"
#include "QEncoder.hpp"
#include "ESC_Serial.hpp"

#define CLOSED_LOOP_MODE 1

#define VIRTUAL_POSITION_RESET_TIME_MS 80

const pid_config waste(0.3, //P
                       0.2, //I
                       0,   //D
                       0.5, //I_MAX_CHANGE_PER_CYCLE
                       1,   //D_WINDOW_FAST
                       2);  //D_WINDOW_SLOW

const pid_config torque(0.62,   //P
                        0.47,   //I
                        0,      //D
                        0.4,    //I_MAX_CHANGE_PER_CYCLE
                        1,      //D_WINDOW_FAST
                        2);     //D_WINDOW_SLOW


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
    PID::initialize_all(waste, torque);

    //Initialize current measuring ADCs
    ADC_Motor::initialize_all();

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

void ESC::update()
{
    for (uint8_t motor = 0; motor < NUM_MOTORS; motor++)
    {
        //Get phase A and B currents 
        //Calculate current C via Kirchoffs current law
        pwm_phases phases {0, 0, 0};
        ADC_Motor::get((motor_id)motor)
                  .get_samples(phases.A, phases.A);
        phases.C = - phases.C - phases.A;

        //Get virtual angle of rotor
        uint32_t virtual_angle = QEncoder::get((motor_id)motor)
                                          .get_virtual_position();
        
        //Transform phases to rotating reference frame
        Idq waste_torque = Transform::de_phase(virtual_angle, phases);

        //PID stuff
        //and also speed and direction stuff

        //Transform rotating reference frame to phases
        phases = Transform::to_phase(virtual_angle, waste_torque);
        
        //Stage PWM output to be updated
        PWM::get((motor_id)motor).set_phases(phases);
    }
}