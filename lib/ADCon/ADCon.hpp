#pragma once
#include <driver/adc.h>


class ADCon 
{
public:

    adc1_channel_t Channel;
    adc_atten_t SampleRange;
    adc_bits_width_t SampleSize;

    ADCon(adc1_channel_t Channel, 
          adc_atten_t SampleRange, 
          adc_bits_width_t Samplesize);

private:    
    static const float ADC_Resolution;
    static const float ADCResistor;

public:
    float Sample();

};
