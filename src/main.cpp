#include <Arduino.h>

#include "Transform.hpp"
#include "PID.hpp"
#include "Motor.hpp"

//PID and transform
uint16_t angle = 0;

Idq dq;
Iabc abc;

PID_Controller PID_waste(0.3, 0.2, 0, 0.5);
PID_Controller PID_torque(0.62, 0.47, 0, 0.4);


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
void IRAM_ATTR M0HandleInterrupt() 
{
    portENTER_CRITICAL_ISR(&mux);

        M0stateB = digitalRead(22);

        if(M0stateB == 1){   //Clockwise rotation
            posX++;
            M0RotaryPos++;
        } else if(M0stateB == 0){  //Counter Clockwise rotation
            posX--;
            M0RotaryPos--;
        }

        if(M0RotaryPos == 343)
        {
            M0RotaryPos = 0;
        } 
        else if(M0RotaryPos == 4294967295)  //Max value of uint32_t
        { 
            M0RotaryPos = 342;
        }

    portEXIT_CRITICAL_ISR(&mux);

}    

// Handle Interrupt from motor 1
void IRAM_ATTR M1HandleInterrupt() 
{
    portENTER_CRITICAL_ISR(&mux);

        M1stateB = digitalRead(16);

        if(M1stateB == 1){   //Clockwise rotation
            posY++;
            M1RotaryPos++;
        } else if(M1stateB == 0){  //Counter Clockwise rotation
            posY--;
            M1RotaryPos--;
        }

        if(M1RotaryPos == 343)
        {
            M1RotaryPos = 0;
        } 
        else if(M1RotaryPos == 4294967295)  //Max value of uint32_t
        { 
            M1RotaryPos = 342;
        }

    portEXIT_CRITICAL_ISR(&mux);

}

void setup()
{
    Serial.begin(9600);

    abc.Ia = 0;
    abc.Ib = 0;
    abc.Ic = 0;

    Motor0::get().initialize().set_pwm(0, 0, 0);

    pinMode(InterruptPinM0, INPUT_PULLUP);    //Set interrupt pin to internal pullup
    pinMode(22, INPUT);
    attachInterrupt(digitalPinToInterrupt(InterruptPinM0),M0HandleInterrupt, RISING); //Trigger interrupt on pin 23, on a rising edge
    
    pinMode(InterruptPinM1, INPUT_PULLUP);    //Set interrupt pin to internal pullup
    pinMode(16, INPUT);
    attachInterrupt(digitalPinToInterrupt(InterruptPinM1),M1HandleInterrupt, RISING); //Trigger interrupt on pin 23, on a rising edge
}

<<<<<<< HEAD
void loop() 
{
    dq = Transform::de_phase(angle, abc);

    dq.Id = PID_waste.update(dq.Id, (float)0);
    dq.Iq = PID_torque.update(dq.Iq, (float)60);

    abc = Transform::to_phase(angle, dq);

    abc.Ia = abc.Ia +3;
    abc.Ib = abc.Ib -4;
    abc.Ic = abc.Ic +2;

    Motor0::get().set_pwm(abc.Ia, abc.Ib, abc.Ic);
    

    // Serial.print(abc.Ia);
    // Serial.print(',');
    // Serial.print(abc.Ib);
    // Serial.print(',');
    // Serial.print(abc.Ic);
    // Serial.print(',');

    // Serial.print(dq.Id);
    // Serial.print(',');
    // Serial.println(dq.Iq);

    angle = (uint16_t)(M0RotaryPos);
    // ((angle < MECHANICAL_ROTATION_STEPS))
    //     ? angle++
    //     : angle = 0;
    // Serial.println((uint16_t)(posX/60 % (MECHANICAL_ROTATION_STEPS)));
    // Serial.println(angle);
    // Serial.println("---");
    // delayMicroseconds(1000);

    // Serial.println("-----------------");
    // Serial.println(posX);
    // Serial.println(posY);
    // Serial.println(posX - posY);
    // delay(1);
=======
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
>>>>>>> Update_Real_Position

}

