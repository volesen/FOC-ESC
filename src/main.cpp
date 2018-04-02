#include <driver/adc.h>
#include <Arduino.h>

//Variables
float Motor0_CurrentA = 0; 
float Motor0_CurrentB = 0;
float Motor1_CurrentA = 0; 
float Motor1_CurrentB = 0;
float ADC_Resolution = 0.0008;

float ADCResistor = 0.12;

int read_raw;
int read_raw2;  

//Initialize ADC1 and ADC2
void ADCInit() {
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);

    adc2_config_channel_atten(ADC2_CHANNEL_0, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(ADC2_CHANNEL_1, ADC_ATTEN_DB_11);

}

//Get ADC value and calculate Current
void ADCGetSample() {
    Motor0_CurrentA = (adc1_get_raw(ADC1_CHANNEL_0)*ADC_Resolution)/ADCResistor;
    Motor0_CurrentB = (adc1_get_raw(ADC1_CHANNEL_3)*ADC_Resolution)/ADCResistor;

    esp_err_t r = adc2_get_raw( ADC2_CHANNEL_0, ADC_WIDTH_12Bit, &read_raw);
    if ( r == ESP_OK ) {
        Motor1_CurrentA = (read_raw*ADC_Resolution)/ADCResistor;
    } else if ( r == ESP_ERR_TIMEOUT ) {
        printf("Error, ADC2 currently used by WiFi");
    }

    esp_err_t i = adc2_get_raw( ADC2_CHANNEL_1, ADC_WIDTH_12Bit, &read_raw2);
    if ( i == ESP_OK ) {
        Motor1_CurrentB = (read_raw2*ADC_Resolution)/ADCResistor;
    } else if ( i == ESP_ERR_TIMEOUT ) {
        printf("Error, ADC2 currently used by WiFi");
    }
}    

void setup(){
    ADCInit();
    
}

void loop() {
    ADCGetSample();  

}
