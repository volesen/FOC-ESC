#include <Arduino.h>
#include "ADCon.hpp"

void setup()
{
    Serial.begin(9600);

}

ADCon ADC_M0_A(0);
ADCon ADC_M0_B(1);
ADCon ADC_M1_A(2);
ADCon ADC_M1_B(3);

void loop() 
{
    Serial.println(ADC_M0_A.get_sample()); //Sample Current through wire to Motor 0 Phase A


    delay(300);
}



