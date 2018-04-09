#include <Arduino.h>

#include "Transform.hpp"
#include "PID.hpp"
#include "Motor.hpp"
#include "QEncoder.hpp"

//PID and transform
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

    Motor::initialize_all();
    QEncoder::initialize_all();

    Motor::get(0).set_pwm(0, 0, 0);
}

void loop() 
{
    // dq = Transform::de_phase(angle, abc);

    // dq.Id = PID_waste.update(dq.Id, (float)0);
    // dq.Iq = PID_torque.update(dq.Iq, (float)60);

    // abc = Transform::to_phase(angle, dq);

    // abc.Ia = abc.Ia +3;
    // abc.Ib = abc.Ib -4;
    // abc.Ic = abc.Ic +2;

    // Motor0::get().set_pwm(abc.Ia, abc.Ib, abc.Ic);
    

    // Serial.print(abc.Ia);
    // Serial.print(',');
    // Serial.print(abc.Ib);
    // Serial.print(',');
    // Serial.print(abc.Ic);
    // Serial.print(',');

    // Serial.print(dq.Id);
    // Serial.print(',');
    // Serial.println(dq.Iq);

    // angle = (uint16_t)(M0RotaryPos);
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

    Serial.println(QEncoder::get(0).get_axis_position());

    // delay(300);

}

