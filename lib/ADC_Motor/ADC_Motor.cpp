#include "ADC_Motor.hpp"

ADC_Motor::ADC_Motor(ADC_PHASE_PAIR pair)
    : phase_A(pair == ADC_PHASE_PAIR::Motor0 ? ADC_PIN::M0_PHASE_A : ADC_PIN::M1_PHASE_A),
      phase_B(pair == ADC_PHASE_PAIR::Motor0 ? ADC_PIN::M0_PHASE_B : ADC_PIN::M1_PHASE_B)
{

}

void ADC_Motor::get_samples(float &out_phase_A, float &out_phase_B)
{
    phase_A.start_sampling();
    phase_B.start_sampling();

    out_phase_A = phase_A.get_sample();
    out_phase_B = phase_B.get_sample();
}