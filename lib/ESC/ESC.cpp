#include <Arduino.h>

#include "ESC.hpp"

#include "ESC_Serial.hpp"
#include "PID.hpp"
#include "ADC_Motor.hpp"
#include "QEncoder.hpp"
#include "PWM.hpp"
#include "Transform.hpp"

#define VIRTUAL_POSITION_RESET_TIME_MS 500

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

    for (uint8_t motor = 0; motor < NUM_MOTORS; motor++)
    {
        // Align rotors so that virtual position is known
        reset_rotor_virtual_position((motor_id)motor);

        // Zero position so that physical position is aligned with axis_position field
        reset_axis_position((motor_id)motor);

        //Set initial target_position to 0
        target_position[motor] = DEFAULT_START_POSITION;
    }
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

void ESC::reset_rotor_virtual_position(motor_id motor)
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
    if (PWM::get(motor).get_phases_max_bound() != 0)
        //Restore saved phases
        PWM::get(motor).set_phases(previous_phases);
    else
        //Set low if PWM::set_phase hasn't been called yet for values
        //This avoid a bug at startup where PWM is low before entering this function,
        //but becomes 50 % (neutral) after leaving
        PWM::get(motor).set_phases_low();
}

void ESC::reset_axis_position(motor_id motor)
{
    //TODO: Implement initial zeroing procedure

    //Reset axis position field in QEncoder class
    QEncoder::get(motor).reset_axis_position();
}

#pragma endregion

ESC& ESC::get() { static ESC instance{}; return instance; }
void ESC::initialize() { get(); }

#if DEBUG_MODE == 0
void ESC::update()
{
    for (uint8_t id = 0; id < NUM_MOTORS; id++)
    {
        //Convert id to motor_id
        motor_id motor = (motor_id)id;

        //Get phase A and B currents 
        //Calculate current C via Kirchoff's current law
        pwm_phases phases {0, 0, 0};
        ADC_Motor::get(motor)
                  .get_samples(phases.A, phases.B);
        phases.C = -phases.A-phases.B;

        //Get virtual angle of rotor
        uint32_t virtual_angle = QEncoder::get(motor)
                                          .get_virtual_position();

        //Get axis position of motor
        uint32_t position = QEncoder::get(motor)
                                     .get_axis_position();

        //Stage update of axis position to ESC_Serial
        ESC_Serial::get()
                   .update_position(motor, position);
        
        //Update target position
        if (ESC_Serial::get().ask_updated(motor))
            target_position[motor] = position + ESC_Serial::get().get_position_change(motor);

        //Transform phases to rotating reference frame
        Idq waste_torque = Transform::de_phase(virtual_angle, phases);

        //Update PI-loops
        waste_torque.d = PID::get(motor).waste.update(waste_torque.d, 0);
        // This is not overflow safe. 
        // Position and relative position control is inherently flawed against overflow.
        // General protection will ruin the PID-loop.
        // Everything works without overflow protection, though, as long as no overflows happen.
        waste_torque.q = PID::get(motor).torque.update(position, target_position[motor]);
        
        //Transform rotating reference frame to phases
        phases = Transform::to_phase(virtual_angle, waste_torque);
        
        //Stage PWM output to be updated
        PWM::get(motor).set_phases(phases);
    }
}
#elif DEBUG_MODE == 1
void ESC::update()
{
    static uint16_t virtual_angle = 0;
    static pwm_phases phases {0, 0, 0};
    static float throttle = 40;

    Idq waste_torque = Transform::de_phase(virtual_angle, phases);

    // throttle = ESC_Serial::get().get_throttle(motor1);

    // throttle = sin((float)virtual_angle / (ONE_VIRTUAL_ROTATION_STEPS - 1.0) * 2.0 * PI) + 1;

    waste_torque.d += PID::get(motor1).waste.update(waste_torque.d, 0);
    waste_torque.q += PID::get(motor1).torque.update(waste_torque.q, throttle);

    phases = Transform::to_phase(virtual_angle, waste_torque);
    // Serial.println(throttle);
    // phases.A += 9;
    // // phases.A *= 1.1;

    // phases.C += -20;
    // phases.C *= 0.98;
    
    Serial.print(Transform::to_phase(0, Idq {0, 40}).A); Serial.print(",");
    Serial.print(Transform::to_phase(0, Idq {0, 40}).B); Serial.print(",");
    Serial.println(Transform::to_phase(0, Idq {0, 40}).C);

    // Serial.print(a); Serial.print(",");
    // Serial.print(b); Serial.println();

    

    // Serial.print(waste_torque.d); Serial.print(",");
    // Serial.print(waste_torque.q); Serial.print(",");
    // Serial.print(phases.A); Serial.print(",");
    // Serial.print(phases.B); Serial.print(",");
    // Serial.print(phases.C); Serial.println();

    PWM::get(motor1).set_phases(phases);

    virtual_angle = ++virtual_angle % ONE_VIRTUAL_ROTATION_STEPS;
}

#endif