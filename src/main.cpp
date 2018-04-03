#include <Arduino.h>

#include "Motor.hpp"
#include "PID.hpp"

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.println("going to initialize now");

    float error = 3;
    float ref = 93;

    PID_Controller troll(1.1, 0.6, 0.45, 30, 1,2);

    motor_pwm signal(0, 0, 0);
    
    Motor0::get().initialize();
    
    Serial.println("running tests now");

    for (int32_t i = 0; i < 80; i++)
    {
        // Serial.println(error);
        error += troll.update(error, ref) + 16;
        
        signal.A = error;

        Motor0::get().set_pwm(signal);

        delay(100);
    }

    Serial.println("finished");
}

void loop() {
    // put your main code here, to run repeatedly:
}