#pragma once

#include <stdint.h>
#include <arm_math.h>

struct motor_outputs
{
		float32_t A, B, C;

        motor_outputs get_scaled() const;      //Return scaled outputs within valid PWM ranges
};

class IMotor
{
    protected:
        static motor_outputs _outputs;

        virtual static void set_pwm(const motor_outputs &outputs) = 0;

	public:
        //Methods
        virtual static motor_outputs get_outputs() = 0;

        virtual static void initialize() = 0;
        virtual static void update_pwm(const motor_outputs &outputs) = 0;
};

class Motor1 : public IMotor
{
    protected:
        static motor_outputs _outputs;

        virtual static void set_pwm(const motor_outputs &outputs) override;        //Updates the PWM output at the motor pins

    public:
        //Methods
        virtual static motor_outputs get_outputs() override;
        
        virtual static void initialize() override;
        virtual static void update_pwm(const motor_outputs &outputs) override;     //Scales the output then passes to set_pwm
};