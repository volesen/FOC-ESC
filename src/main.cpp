#include <Arduino.h>

#include "Transform.hpp"
#include "PID.hpp"
#include "Motor.hpp"

uint16_t angle = 0;

Idq dq;
Iabc abc;

PID_Controller PID_waste(0.3, 0.2, 0, 0.5);
PID_Controller PID_torque(0.62, 0.47, 0, 0.4);

void setup()
{
    Serial.begin(9600);

    abc.Ia = 0;
    abc.Ib = 0;
    abc.Ic = 0;

    //Motor0::get().initialize().
}

void loop() 
{
    dq = Transform::de_phase(angle, abc);

    dq.Id = PID_waste.update(dq.Id, (float)0);
    dq.Iq = PID_torque.update(dq.Iq, (float)60);

    abc = Transform::to_phase(angle, dq);

    abc.Ia = abc.Ia +3;
    abc.Ib = abc.Ib -4;
    abc.Ic = abc.Ic +2;
    



    // Serial.print(abc.Ia);
    // Serial.print(',');
    // Serial.print(abc.Ib);
    // Serial.print(',');
    // Serial.print(abc.Ic);
    // Serial.print(',');

    // Serial.print(dq.Id);
    // Serial.print(',');
    // Serial.println(dq.Iq);

    ((angle < MECHANICAL_ROTATION_STEPS))
        ? angle++
        : angle = 0;
}

