#include <Arduino.h>
#include "ADCon.hpp"

ADCon Motor0(ADC1_CHANNEL_0, ADC_ATTEN_DB_11, ADC_WIDTH_12Bit);
ADCon Motor1(ADC1_CHANNEL_3, ADC_ATTEN_DB_11, ADC_WIDTH_12Bit);

void setup()
{
    Serial.begin(9600);    
}

void loop() 
{
    Serial.println(Motor0.Sample());
    Serial.println("------------");
    Serial.println(Motor1.Sample());

}
