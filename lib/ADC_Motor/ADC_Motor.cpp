#include "ADC_Motor.hpp"

ADC_Motor::ADC_Motor(motor_id motor)
    : phase_A(motor == motor0 ? ADC_PIN::M0_PHASE_A : ADC_PIN::M1_PHASE_A),
      phase_B(motor == motor0 ? ADC_PIN::M0_PHASE_B : ADC_PIN::M1_PHASE_B)
{

}

void ADC_Motor::initialize_all()
{
    get(motor0);
    get(motor1);
}

ADC_Motor& ADC_Motor::get(motor_id motor)
{
    //I do not feel like protecting against people deleting the object the pointer points to
    //This seems unnecessary given the circumstances the code is going to be used in.
    switch (motor)
    {
        case motor0:
        {   //The brackets are used to create a new scope so that the variable instance can be used multiple times
            static ADC_Motor instance(motor);

            return instance;
        }
            
        case motor1:
        {
            static ADC_Motor instance(motor);

            return instance;
        }

        default:
            //TODO: go to error handler or something
            break;
    }
}

void ADC_Motor::get_samples(float &out_phase_A, float &out_phase_B)
{
    phase_A.start_sampling();
    phase_B.start_sampling();

    out_phase_A = phase_A.get_sample();
    out_phase_B = phase_B.get_sample();
}