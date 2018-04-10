#include "ADCon.hpp"
#include <Arduino.h>

#define ADC_RESOLUTION 0.0002686
#define ADC_RESISTOR 0.12
#define DAMPMultiply 10

#define ADC_ATTEN 0
#define ADC_WIDTH 12
#define ADC_CYCLES 8
#define ADC_SAMPLES 1
#define ADC_CLK_DIV 1

ADCon::ADCon(ADC_PIN pin)
    : pin((uint8_t)pin), sample(0), first_run(true), sample_updated(false)
{
    analogSetWidth(ADC_WIDTH);
    analogSetCycles(ADC_CYCLES);
    analogSetSamples(ADC_SAMPLES);
    analogSetClockDiv(ADC_CLK_DIV);
    analogSetAttenuation((adc_attenuation_t)ADC_ATTEN);

    pinMode(this->pin, INPUT);
    adcAttachPin(this->pin);
}

float ADCon::convert_sample(const uint32_t &sample)
{
    return sample * ADC_RESOLUTION / DAMPMultiply / ADC_RESISTOR;
}
bool ADCon::ask_sample_updated() 
{ 
    return sample_updated; 
}
bool ADCon::ask_sampling()
{
    return adcBusy(pin);
}
void ADCon::wait_save_restart_sample()
{
    sample = convert_sample(adcEnd(pin));
    sample_updated = true;
    adcStart(pin);
}

float ADCon::get_sample()
{
    if (first_run)                      //If first time getting
    {
        adcStart(pin);                  //Start sampling
        wait_save_restart_sample();     //Wait for sample to finish, save sample, restart sampling
        
        first_run = false;              //Mark first_run as false to not run again.
    }
    else if (!adcBusy(pin))             //If done sampling
        wait_save_restart_sample();     //Save sample and restart sampling
    else
        sample_updated = false;         //Mark return value as old

    return sample;
}
