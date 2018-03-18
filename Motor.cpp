#include <stdint.h>

#include "Motor.hpp"

//Constructor
Motor::Motor(motor_pins)
{

}

float32_t Motor::get_motor_output_max(const motor_outputs &outputs)
{
		
	
    if (outputs.A >)
}

//Scales to a 16-bit value
motor_outputs Motor::scale_outputs(const motor_outputs &outputs)
{
    //Find maximum output
    float32_t output_max = get_motor_output_max(outputs);

    outputs.A = 
}

void Motor::set_pwm(motor_outputs outputs)
{

}

void Motor::update_pwm(motor_outputs outputs)
{
    set_pwm(scale_outputs(outputs));
}