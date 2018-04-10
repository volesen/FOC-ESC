#pragma once
#include <driver/adc.h>

enum class ADC_PIN
{
    M0_PHASE_A = 36,
    M0_PHASE_B = 4,
    M1_PHASE_A = 39,
    M1_PHASE_B = 0
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

        static float convert_sample(const uint16_t &sample);
        void wait_save_restart_sample();

    public:
        bool ask_sample_updated();
        float get_sample();
};
