#include "ADCon.hpp"
#include <Arduino.h>

#define ADC_RESOLUTION 0.0002686
#define ADC_RESISTOR 0.12
#define DAMPMultiply 10

#define ADC_SAMPLE_RANGE ADC_ATTEN_DB_0
#define ADC_SAMPLE_SIZE ADC_WIDTH_12Bit

ADCon::ADCon(char channel_id)
    : saved_sample(0)
{
    adc_power_on();
    adc_set_clk_div(1);

    switch (channel_id)
    {
        case 0:
            channel = ADC1_CHANNEL_0;
            break;
            
        case 1:
            channel = ADC1_CHANNEL_3;
            break;

        case 2:
            channel = ADC1_CHANNEL_6;
            break;

        case 3:
            channel = ADC1_CHANNEL_7;
            break;

        default:
            //throw error or something
            return;
    }

    adc1_config_width(ADC_SAMPLE_SIZE);
    adc1_config_channel_atten(channel, ADC_SAMPLE_RANGE);

    pinMode(channel, INPUT);
}

float ADCon::get_sample()
{
    saved_sample = adc1_get_raw(channel) * ADC_RESOLUTION/DAMPMultiply/ADC_RESISTOR;
    
    return saved_sample;
}

float ADCon::get_saved_sample() { return saved_sample; }

adc1_channel_t ADCon::get_channel() { return channel; }
