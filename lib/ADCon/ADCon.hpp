#pragma once
#include <driver/adc.h>


class ADCon 
{
public:
    ADCon(char channel_id);

private:
    adc1_channel_t channel;
    float saved_sample;

  public:
    float get_sample();
    float get_saved_sample();

    adc1_channel_t get_channel();
};
