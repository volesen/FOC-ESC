#include "ADCon.hpp"
#include <Arduino.h>

const float ADCon::ADC_Resolution = 0.0002686;
const float ADCon::ADCResistor = 0.12;
const float ADCon::DAMPMultiply = 10;

ADCon::ADCon(adc1_channel_t Channel, 
            adc_atten_t SampleRange, 
            adc_bits_width_t SampleSize)
    : Channel(Channel), SampleRange(SampleRange), SampleSize(SampleSize) 
{
    adc1_config_width(SampleSize);
    adc1_config_channel_atten(Channel, SampleRange);
    pinMode(Channel,INPUT); 
}

float ADCon::Sample()
{
    float ADCVal = adc1_get_raw(Channel);
    ADCVal = ((ADCVal * ADC_Resolution)/DAMPMultiply)/ADCResistor;
    return (ADCVal);
}    
