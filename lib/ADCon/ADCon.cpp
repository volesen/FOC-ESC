#include "ADCon.hpp"
#include <Arduino.h>

const float ADCon::ADC_Resolution = 0.0002686;
const float ADCon::ADCResistor = 1.0;
const float ADCon::DAMPMultiply = 10;

const adc_atten_t SampleRange = ADC_ATTEN_DB_0;
const adc_bits_width_t SampleSize = ADC_WIDTH_12Bit;

ADCon::ADCon(char channel_id)
{
    adc_power_on();
    adc_set_clk_div(1);

    switch (channel_id)
    {
        case 0:
            channel = ADC1_CHANNEL_0;
        case 1:
            channel = ADC1_CHANNEL_3;
        case 2:
            channel = ADC1_CHANNEL_6;
        case 3:
            channel = ADC1_CHANNEL_7;
        default:
            //throw error or something
            return;
    }

    adc1_config_width(SampleSize);
    adc1_config_channel_atten(channel, SampleRange);

    pinMode(channel,INPUT); 
}

float ADCon::get_sample()
{
    float ADCVal = adc1_get_raw(channel);
    // ADCVal = ((ADCVal * ADC_Resolution)/DAMPMultiply)/ADCResistor;
    return (ADCVal);
}

adc1_channel_t ADCon::get_channel() { return channel; }

adc_atten_t ADCon::get_sampleRange() { return sampleRange; }
adc_bits_width_t ADCon::get_sampleSize() { return sampleSize; }
