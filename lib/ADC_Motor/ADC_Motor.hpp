#pragma once

#include "ADC_Phase.hpp"

enum class ADC_PHASE_PAIR
{
    Motor0,
    Motor1
};

class ADC_Motor
{
    private:
        ADC_Phase phase_A;
        ADC_Phase phase_B;

      public:
        ADC_Motor(ADC_PHASE_PAIR pair);

        void get_samples(float &phase_A, float &phase_B);
};

//TODO:remember to do so we always only pair sample one motor