#include <driver/adc.h>
#include <Arduino.h>

//Variables
float Motor0_CurrentA = 0; 
float Motor1_CurrentA = 0;

float ADC_Resolution = 0.0008;
float ADCResistor = 0.12;

//Initialize ADC1 and ADC2
void ADCInit() {
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);

    analogSetClockDiv(1); //Set clock divider to improve performance

}

//Get ADC value and calculate Current
void ADCGetSample() {
    Motor0_CurrentA = (adc1_get_raw(ADC1_CHANNEL_0));
    Motor1_CurrentA = (adc1_get_raw(ADC1_CHANNEL_3));

}    

void setup(){
    Serial.begin(9600);
    ADCInit();
        
}

void loop() {

    ADCGetSample();

}
