#include <Arduino.h>

#include <Motor.hpp>

void setup()
{
    Serial.begin(9600);
    Serial.println();
    Serial.println("going to initialize now");
    Motor0::get().initialize();
    Motor1::get().initialize();
    Motor0::get().set_pwm(motor_pwm(-2000, 2000, 0));
    Motor1::get().set_pwm(motor_pwm(2000, 2000, 0));
    motor_pwm stuff = Motor0::get().get_pwm();//.get_descaled(Motor0::get().get_pwm_max_bound());
    Serial.println("hi, i'm initialized");

    // Serial.begin(9600);
    // initialize LED digital pin as an output.
    // for (int i = 0; i < 2; i++)
    // {
        //pinMode(1, OUTPUT);
        // digitalWrite(1, LOW);
    // }



}

void loop()
{
    // turn the LED on (HIGH is the voltage level)
    //digitalWrite(1, LOW);
    delay(500);
    //Serial.print("hi");
}