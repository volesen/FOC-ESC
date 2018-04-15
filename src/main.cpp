#include <Arduino.h>

#include "ESC.hpp"

uint32_t iteration = 0;

//PID and transform
uint16_t angle = 0;


void setup()
{
    // ASerial::get().update_position(18531, motor_id::motor1);
    //SERIAL USB PIN ON YOUR THING IS 1... or maybe UART0
    // Serial.begin(9600);
    // Serial.println();
    // Serial.println();
    // Serial.println();
    
    // Serial.println(get_bit(a, 0));
    // Serial.println(get_bit(a, 7));
    // Serial.println(get_bit(a, 6));

    // pinMode(1, OUTPUT);

    // abc.Ia = 0;
    // abc.Ib = 0;
    // abc.Ic = 0;

    // Motor::initialize_all();
    // QEncoder::initialize_all();
    
    // Serial.println(xPortGetCoreID());
    // Motor::get(0).initialize();
    // QEncoder::get(0);


    //Set PWM of Motor0 in case of emergency
    // Motor::get(0).initialize().set_pwm(1, 1, 1);

    // float pa, pb;

    // ADC_Motor0.get_samples(pa, pb);

    // // float t = ADC_M0_A.get_sample();

    // Serial.print(pa);
    // Serial.print(" ");
    // Serial.print(pb);

    // adcStart((uint8_t)ADC_PIN::M0_PHASE_A);
    // adcStart((uint8_t)ADC_PIN::M0_PHASE_A);
    // delay(1);
    // bool t = adcBusy((uint8_t)ADC_PIN::M0_PHASE_A);
    // ADC_M0_A.get_sample();
    // ADC_M0_A.get_sample();
    // Serial.println(t);

    // ADC_M0_A.get_sample();
    // ADC_M0_B.get_sample();
    // ADC_M1_A.get_sample();
    // ADC_M1_B.get_sample();

    // for (int m = 0; m < 5; m++)
    // {
        // uint32_t start = micros();

        // float a, b, c, d;
        // c = d = 0;

        // for (int i = 0; i < 40000; i++)
        // {
        //     ADC_Motor0.get_samples(a, b);
        //     c += a;
        //     d += b;

        //     ADC_Motor1.get_samples(a, b);
        //     c += a;
        //     d += b;
        // }

        // Serial.println("----------------");
        // //Frequency of full cycle samples given in kHz
        // Serial.println(40*1000/((micros() - start)*pow10(-6)));
        // //Average value
        // Serial.println(c / 40000);
        // Serial.println(d / 40000);
    // }
}

void loop() 
{
    // Serial.println("loop");
    // ASerial::get();
    // ASerial::get().update_scope_buffer();

    // for (int i = 0; i < ASerial::get().scope_buffer_data_available; i++)
    //     Serial.println(ASerial::get().scope_buffer[i]);

    // Serial.println(xPortGetCoreID());

    // Serial.println(ASerial::get().get_speed(motor_id::motor1));
    // Serial.println((149 & 63) | ((90 & 63) << 6));
    delay(1000);
    

    // digitalWrite(1, HIGH);
    // delay(500);
    // digitalWrite(1, LOW);
    // delay(500);

    // dq = Transform::de_phase(angle, abc);

    // dq.Id = PID_waste.update(dq.Id, (float)0);
    // dq.Iq = PID_torque.update(dq.Iq, (float)60);

    // abc = Transform::to_phase(angle, dq);

    // abc.Ia = abc.Ia +3;
    // abc.Ib = abc.Ib -4;
    // abc.Ic = abc.Ic +2;

    // Motor0::get().set_pwm(abc.Ia, abc.Ib, abc.Ic);
    
    // float a = ADC_M0_A.get_sample();
    // if (ADC_M0_A.ask_sample_updated() && a > 0)
    //     Serial.println(a);

    // Serial.print(abc.Ia);
    // Serial.print(',');
    // Serial.print(abc.Ib);
    // Serial.print(',');
    // Serial.print(abc.Ic);
    // Serial.print(',');

    // Serial.print(dq.Id);
    // Serial.print(',');
    // Serial.println(dq.Iq);

    // angle = (uint16_t)(angle);
    // ((angle < MECHANICAL_ROTATION_STEPS))
    //     ? angle++
    //     : angle = 0;
    // Serial.println((uint16_t)(posX/60 % (MECHANICAL_ROTATION_STEPS)));
    // Serial.println(angle);
    // Serial.println("---");
    // delayMicroseconds(1000);


}



