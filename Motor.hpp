#pragma once

#include <stdint.h>

struct motor_pins
{
    uint32_t A, B, C;
};

struct motor_outputs
{
		float32_t A, B, C;
};

class Motor
{
    private:
        motor_pins _pins;
        motor_outputs _outputs;

        static float32_t get_motor_output_max(const motor_outputs &outputs);

    public:
        //Static methods
        static motor_outputs scale_outputs(const motor_outputs &outputs);      //Scale outputs to match

        //Constructor
        Motor(const motor_pins &pins);

        //Destructor
        ~Motor();       //Stop PWM output

        //Methods
        void set_pwm(const motor_outputs &outputs);        //Updates the PWM output at the motor pins
        void update_pwm(const motor_outputs &outputs);     //Scales the output then passes to set_pwm


};