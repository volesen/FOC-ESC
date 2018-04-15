#include "ADC_Motor.hpp"

ADC_Motor::ADC_Motor(motor_id motor)
    : phase_A(motor == motor_id::motor0 ? ADC_PIN::M0_PHASE_A : ADC_PIN::M1_PHASE_A),
      phase_B(motor == motor_id::motor0 ? ADC_PIN::M0_PHASE_B : ADC_PIN::M1_PHASE_B)
{

}

void ADC_Motor::get_samples(float &out_phase_A, float &out_phase_B)
{
    phase_A.start_sampling();
    phase_B.start_sampling();

    out_phase_A = phase_A.get_sample();
    out_phase_B = phase_B.get_sample();
}