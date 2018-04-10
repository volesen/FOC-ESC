#pragma once

#include <stdint.h>

enum class ADC_PIN
{
    M0_PHASE_A = 36,
    M0_PHASE_B = 2,
    M1_PHASE_A = 39,
    M1_PHASE_B = 15
};

class ADCon 
{
    public:
        ADCon(ADC_PIN pin);

    private:
        uint8_t pin;
        float sample;
        bool first_run;
        bool sample_updated;

        static float convert_sample(const uint32_t &sample);
        void wait_save_restart_sample();

    public:
        bool ask_sample_updated();
        bool ask_sampling();
        float get_sample();
};
