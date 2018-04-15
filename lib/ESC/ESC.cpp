#include <Arduino.h>

#include "ESC.hpp"

#include "Transform.hpp"
#include "Motor.hpp"
#include "QEncoder.hpp"
#include "ASerial.hpp"

#define CLOSED_LOOP_MODE 1

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



ESC::ESC()
{
    initialize_classes();

    reset_rotor_virtual_position();
}

void ESC::initialize_classes()
{
    //Initialize serial interface
    ASerial::initialize();

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
    Motor::initialize_all();
}

void reset_rotor_virtual_position()
{
    
}

ESC& ESC::get()
{
    static ESC instance{};

    return instance;
}