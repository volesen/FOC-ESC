#pragma once

#include "ADC_Phase.hpp"
#include "ESC_Globals.hpp"

class ADC_Motor
{
    private:
        ADC_Phase phase_A;
        ADC_Phase phase_B;

      public:
        ADC_Motor(motor_id motor);

        static void initialize_all();
        static ADC_Motor &get(motor_id motor);

        void get_samples(float &phase_A, float &phase_B);
};

//TODO:remember to do so we always only pair sample one motor