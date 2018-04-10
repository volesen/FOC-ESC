#pragma once
#include <driver/adc.h>


class ADCon 
{
public:
    ADCon(char channel_id);

private:    
    static const float ADC_Resolution;
    static const float ADCResistor;
    static const float DAMPMultiply;

    static const adc_atten_t sampleRange;
    static const adc_bits_width_t sampleSize;

    adc1_channel_t channel;

  public:
    float get_sample();

    adc1_channel_t get_channel();

    static adc_atten_t get_sampleRange();
    static adc_bits_width_t get_sampleSize();
};
