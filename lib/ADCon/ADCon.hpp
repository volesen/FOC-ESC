#pragma once
#include <driver/adc.h>


class ADCon 
{
public:
    ADCon(char channel_id);

private:
    adc1_channel_t channel;

  public:
    float get_sample();

    adc1_channel_t get_channel();
};
