#pragma once

#include <stdint.h>
#include <arm_math.h>


struct motor_pwm
{
	float32_t A, B, C;
	
	motor_pwm(float32_t A = 0, float32_t B = 0, float32_t C = 0);

    motor_pwm get_scaled() const;      //Return scaled outputs within valid PWM ranges
};

class IMotor
{
    protected:
        IMotor() {}
        IMotor(IMotor const &);
        void operator=(IMotor const &);
		
        static bool _initialized;
        motor_pwm _pwm;

        virtual void update_pwm() = 0;

	public:
        //Methods
        virtual void initialize() = 0;

        virtual motor_pwm get_pwm() = 0;
        virtual void set_pwm(const motor_pwm &pwm) = 0;
};

class Motor0 : public IMotor
{
    protected:
        Motor0() {}
        Motor0(Motor0 const &);
        void operator=(Motor0 const &);

        static bool _initialized;
        motor_pwm _pwm;
		
        virtual void update_pwm(); //Updates the PWM output at the motor pins
        
    public:
        //Methods
        static Motor0& get_motor();
        virtual void initialize();

        virtual motor_pwm get_pwm();
        virtual void set_pwm(const motor_pwm &pwm); //Scales the output then passes to set_pwm
};

class Motor1 : public IMotor
{
    protected:
        Motor1() {}
        Motor1(Motor0 const &);
        void operator=(Motor1 const &);

        static bool _initialized;
        motor_pwm _pwm;
		
        virtual void update_pwm(); //Updates the PWM output at the motor pins

    public:
        //Methods
        static Motor1& get_motor();
        virtual void initialize();

        virtual motor_pwm get_pwm();
        virtual void set_pwm(const motor_pwm &pwm); //Scales the output then passes to set_pwm
};
