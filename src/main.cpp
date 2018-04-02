#include <Arduino.h>

//Update real position and ISR. Only set up for Motor0

const byte interruptPin = 23;
volatile int interruptCounter = 0;
int numberOfInterrupts = 0;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

int stateB = 0;
unsigned long pos = 0;

//Interrupt sequence
void IRAM_ATTR handleInterrupt() {
    portENTER_CRITICAL_ISR(&mux);
    interruptCounter++;
    portEXIT_CRITICAL_ISR(&mux);
}

void setup(){
    Serial.begin(9600);
    Serial.println("Monitoring interrupts");

    pinMode(interruptPin, INPUT_PULLUP);    //Set interrupt pin to internal pullup
    attachInterrupt(digitalPinToInterrupt(interruptPin),handleInterrupt, RISING); //Trigger interrupt on a rising edge
}

void loop() {
//Handle the interrupt
    if(interruptCounter > 0){
        portENTER_CRITICAL(&mux);
        interruptCounter--;
        portEXIT_CRITICAL(&mux);

        if(digitalRead(22) == 1){   //Clockwise rotation
            pos++;
        } else if (digitalRead(22) == 0){  //Counter Clockwise rotation
            pos--;
        }

        Serial.println(pos);
    }

}

