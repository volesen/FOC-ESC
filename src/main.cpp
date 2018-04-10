#include <Arduino.h>
#include "ADCon.hpp"

uint32_t Time1 = 0;
uint32_t Time2 = 0;
int Samples = 0;

void setup()
{   
    adc_power_on();
    adc_set_clk_div(1);

}

ADCon M0PhaseA(ADC1_CHANNEL_0, ADC_ATTEN_DB_0, ADC_WIDTH_12Bit);
ADCon M0PhaseB(ADC1_CHANNEL_3, ADC_ATTEN_DB_0, ADC_WIDTH_12Bit);
ADCon M1PhaseA(ADC1_CHANNEL_1, ADC_ATTEN_DB_0, ADC_WIDTH_12Bit);
ADCon M1PhaseB(ADC1_CHANNEL_2, ADC_ATTEN_DB_0, ADC_WIDTH_12Bit);

void loop() 
{
M0PhaseA.Sample(); //Sample Current through wire to Motor 0 Phase A
}



