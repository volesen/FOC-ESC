//Values from ADC 0/1
int ADC0_A;
int ADC0_B;
int ADC1_A;
int ADC1_B;

//Motor 1 wire current
int M1A;
int M1B;
int M1C;

//Motor 2 wire current
int M2A;
int M2B;
int M2C;

//Motor wire current multiplication factor
int Motor1Multiply;
int Motor2Multiply;

//Resolution of ADC conversion
float ADC_Resolution;

//Read values from AD conversion register FIFOn

//Calculate current in wire A
if(ADC0_A >=0){
    ADC0_A = ADC0_A*ADC_Resolution;

    M1A = ADC0_A/Motor1Multiply;

    if(ADC1_A >= 0){
        M1B = M1A;
        M1C = 0;
        } else {
        M1C = M1A;
        M1B = 0;    
        }   

    } else {
        M1A = 0;
        ADC1_A = ADC1_A*ADC_Resolution;
        M1B = ADC1_A/Motor1Multiply;
        M1C = M1B;     
}

