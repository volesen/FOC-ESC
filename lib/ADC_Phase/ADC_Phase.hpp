#pragma once

#include <stdint.h>

enum class ADC_PIN
{
    M0_PHASE_A = 36,
    M0_PHASE_B = 2,
    M1_PHASE_A = 39,
    M1_PHASE_B = 15
};

class ADC_Phase 
{
    public:
        ADC_Phase(ADC_PIN pin);

    private:
        uint8_t pin;
        bool sample_requested;

        static float convert_sample(const uint32_t &sample);

        static bool ADC1_locked;
        static bool ADC2_locked;

        static uint8_t convert_pin_adcnum(uint8_t pin);
        static void lock_adc(uint8_t pin);
        static void unlock_adc(uint8_t pin);
        static bool ask_adc_locked(uint8_t pin);

      public:
        void start_sampling();
        bool ask_sample_ready();
        float get_sample();
};
