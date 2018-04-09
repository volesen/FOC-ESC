#include <Arduino.h>

#include "Transform.hpp"
#include "PID.hpp"

uint16_t angle = 0;

Idq dq;
Iabc abc;

PID_Controller PID_waste(0.9, 0.8, 0, 0.7);
PID_Controller PID_torque(0.9, 0.8, 0, 0.7);

void setup()
{
    Serial.begin(9600);

    abc.Ia = 0;
    abc.Ib = 0;
    abc.Ic = 0;
}

void loop() 
{
    dq = Transform::de_phase(angle, abc);

    dq.Id = PID_waste.update(dq.Id, (float)0);
    dq.Iq = PID_torque.update(dq.Iq, (float)50);

    abc = Transform::to_phase(angle, dq);

    abc.Ia = abc.Ia +3;
    abc.Ib = abc.Ib -4;
    abc.Ic = abc.Ic +2;

    Serial.print(abc.Ia);
    Serial.print(',');
    Serial.print(abc.Ib);
    Serial.print(',');
    Serial.print(abc.Ic);
    Serial.print(',');

    Serial.print(dq.Id);
    Serial.print(',');
    Serial.println(dq.Iq);


    angle < MECHANICAL_ROTATION_STEPS
        ? angle++
        : angle = 0;
}
