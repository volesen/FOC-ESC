#include <Arduino.h>

//Update real position and ISR.

const byte InterruptPinM0 = 23;
const byte InterruptPinM1 = 17;
volatile int interruptCounter = 0;
int numberOfInterrupts = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

int M0stateB = 0;
int M1stateB = 0;
unsigned long posX = 0;
unsigned long posY = 0;
uint32_t M0RotaryPos = 0;
uint32_t M1RotaryPos = 0;

        //Handle interrupt from motor 0
void IRAM_ATTR M0HandleInterrupt() {
    portENTER_CRITICAL_ISR(&mux);

        M0stateB = digitalRead(22);

        if(M0stateB == 1){   //Clockwise rotation
            posX++;
            M0RotaryPos++;
        } else if(M0stateB == 0){  //Counter Clockwise rotation
            posX--;
            M0RotaryPos--;
        }

        if(M0RotaryPos == 2048){
            M0RotaryPos = 0;
        } else if(M0RotaryPos == 0){
            M0RotaryPos = 2048;
        }

    portEXIT_CRITICAL_ISR(&mux);

}    

        //Handle Interrupt from motor 1
void IRAM_ATTR M1HandleInterrupt() {
    portENTER_CRITICAL_ISR(&mux);

        M1stateB = digitalRead(16);

        if(M1stateB == 1){   //Clockwise rotation
            posY++;
            M1RotaryPos++;
        } else if(M1stateB == 0){  //Counter Clockwise rotation
            posY--;
            M1RotaryPos--;
        }

        if(M1RotaryPos == 2048){
            M1RotaryPos = 0;
        } else if(M1RotaryPos == 0){
            M1RotaryPos = 2048;
        }

    portEXIT_CRITICAL_ISR(&mux);

}

void setup(){
    Serial.begin(9600);
    Serial.println("Monitoring interrupts");

    pinMode(InterruptPinM0, INPUT_PULLUP);    //Set interrupt pin to internal pullup
    pinMode(22, INPUT);
    attachInterrupt(digitalPinToInterrupt(InterruptPinM0),M0HandleInterrupt, RISING); //Trigger interrupt on pin 23, on a rising edge
    
    pinMode(InterruptPinM1, INPUT_PULLUP);    //Set interrupt pin to internal pullup
    pinMode(16, INPUT);
    attachInterrupt(digitalPinToInterrupt(InterruptPinM1),M1HandleInterrupt, RISING); //Trigger interrupt on pin 23, on a rising edge
}

void loop() {
/*
    Serial.println("-----------------");
    Serial.println(posX);
    Serial.println("------");
    Serial.println(posY);
    Serial.println("------");
    Serial.println(posX - posY);
    delay(1000);


*/

}

