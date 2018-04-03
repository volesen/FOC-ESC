#include <Arduino.h>

#include "Motor.hpp"
#include "PID.hpp"

void setup() {
    Serial.begin(9600);

    float error = 3;
    float ref = 93;

    for (int i = 0; i < 20; i++)
        Serial.println(ref);
    for (int i = 0; i < 40; i++)
        Serial.println(error);

    PID_Controller troll (0.7, 0.6, 0.45, 30, 1,2);

    

    for (int32_t i = 0; i < 80; i++)
    {
        Serial.println(error);
        error += troll.update(error, ref) + 6;
    }
}

void loop() {
    // put your main code here, to run repeatedly:
}