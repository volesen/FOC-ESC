#include <Arduino.h>

#include <Motor.hpp>

void setup()
{
    Serial.begin(9600);
    Serial.println();
    Serial.println("going to initialize now");
    motor_pwm signal(0, 1000, 2000);
    Motor0::get().initialize().set_pwm(signal);
    Motor1::get().initialize().set_pwm(signal);
    //motor_pwm stuff = Motor0::get().get_pwm();//.get_descaled(Motor0::get().get_pwm_max_bound());
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