#include <Arduino.h>

#include "ADC_Phase.hpp"

#define ADC_RESOLUTION 0.0002686
#define ADC_RESISTOR 0.12
#define DAMPMultiply 10

#define ADC_ATTEN 0
#define ADC_WIDTH 12
#define ADC_CYCLES 8
#define ADC_SAMPLES 1
#define ADC_CLK_DIV 1

bool ADC_Phase::ADC1_locked = false;
bool ADC_Phase::ADC2_locked = false;

ADC_Phase::ADC_Phase(ADC_PIN pin)
    : pin((uint8_t)pin), sample_requested(false)
{
    analogSetWidth(ADC_WIDTH);
    analogSetCycles(ADC_CYCLES);
    analogSetSamples(ADC_SAMPLES);
    analogSetClockDiv(ADC_CLK_DIV);
    analogSetAttenuation((adc_attenuation_t)ADC_ATTEN);

    pinMode(this->pin, INPUT);
    adcAttachPin(this->pin);

    adcStart(this->pin);        //To prevent adcBusy from returning true all the time
}

float ADC_Phase::convert_sample(const uint32_t &sample)
{
    return sample * ADC_RESOLUTION / DAMPMultiply / ADC_RESISTOR;
}

void ADC_Phase::start_sampling()
{
    //Skip data overwriting prevention
    //if user requests new sampling on same pin
    if (!sample_requested)
        while (ask_adc_locked(pin)) {}

    lock_adc(pin);
    adcStart(pin);
    sample_requested = true;
}

bool ADC_Phase::ask_sample_ready()
{
    if (!sample_requested)
        return false;
    else
        return !adcBusy(pin);
}

float ADC_Phase::get_sample()
{
    if (sample_requested)
    {
        float sample = adcEnd(pin);

        unlock_adc(pin);
        sample_requested = false;
        
        return convert_sample(sample);
    }
    else
    {
        start_sampling();
        return get_sample();
    }        
}


bool ADC_Phase::ask_adc_locked(uint8_t pin)
{
    return convert_pin_adcnum(pin) == 1 ? ADC1_locked : ADC2_locked;
}

void ADC_Phase::lock_adc(uint8_t pin)
{
    if (convert_pin_adcnum(pin) == 1)
        ADC1_locked = true;
    else
        ADC2_locked = true;
}

void ADC_Phase::unlock_adc(uint8_t pin)
{
    if (convert_pin_adcnum(pin) == 1)
        ADC1_locked = false;
    else
        ADC2_locked = false;
}

uint8_t ADC_Phase::convert_pin_adcnum(uint8_t pin)
{
    switch (pin)
    {
        case (uint8_t)ADC_PIN::M0_PHASE_A:
        case (uint8_t)ADC_PIN::M1_PHASE_A:
            return 1;

        case (uint8_t)ADC_PIN::M0_PHASE_B:
        case (uint8_t)ADC_PIN::M1_PHASE_B:
            return 2;
    }
}