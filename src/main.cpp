#include <Arduino.h>

#include "Transform.hpp"

uint16_t angle = 0;

void setup()
{
    Serial.begin(9600);
}

void loop() 
{
    Idq dq;

    dq.Id = 9;
    dq.Iq = 569;

    Iabc abc = Transform::to_phase(angle, dq);

    Serial.print(abc.Ia);
    Serial.print(',');
    Serial.print(abc.Ib);
    Serial.print(',');
    Serial.println(abc.Ic);

    dq = Transform::de_phase(angle, abc);

    // Serial.print(dq.Iq);
    // Serial.print(',');
    // Serial.println(dq.Id);

    angle < MECHANICAL_ROTATION_STEPS
        ? angle++
        : angle = 0;
}
