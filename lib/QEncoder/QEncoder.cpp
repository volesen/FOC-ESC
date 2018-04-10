#define Encoder0_pin_int 19
#define Encoder0_pin_sec 18

#define Encoder1_pin_int 5
#define Encoder1_pin_sec 17

#include <Arduino.h>
#include "QEncoder.hpp"
#include "Transform.hpp"

QEncoder::~QEncoder()
{
    detachInterrupt(digitalPinToInterrupt(pin_interrupt));
}

QEncoder::QEncoder(char pin_interrupt, char pin_secondary, void (*interrupt_handler)())
    : pin_interrupt(pin_interrupt), pin_secondary(pin_secondary), 
      axis_position(0), virtual_position(0)
{
    pinMode(pin_interrupt, INPUT);       //TODO:Test if pull down works instead
    pinMode(pin_secondary, INPUT);

    attachInterrupt(digitalPinToInterrupt(      //Pin to interrupt on
                    pin_interrupt),                      
                    interrupt_handler,          //Interrupt handler method
                    1);                         //Interrupt on rising edg
}

void QEncoder::initialize_all()
{
    get(0);
    get(1);
}

QEncoder& QEncoder::get(char id)
{
    //I do not feel like protecting against people deleting the object the pointer points to
    //This seems unnecessary given the circumstances the code is going to be used in.
    switch (id)
    {
        case 0:
        {   //The brackets are used to create a new scope so that the variable Encoder can be used multiple times
            static QEncoder Encoder(Encoder0_pin_int, Encoder0_pin_sec, handle_interrupt_encoder0);

            return Encoder;
        }
            
        case 1:
        {
            static QEncoder Encoder(Encoder1_pin_int, Encoder1_pin_sec, handle_interrupt_encoder1);

            return Encoder;
        }

        default:
            //TODO: Throw an error or something
            break;
    }
}


void IRAM_ATTR QEncoder::handle_interrupt_encoder0()
{
    handle_interrupt(&get(0));
}

void IRAM_ATTR QEncoder::handle_interrupt_encoder1()
{
    handle_interrupt(&get(1));
}

void IRAM_ATTR QEncoder::handle_interrupt(QEncoder* const encoder)
{
    bool state = digitalRead(encoder->pin_secondary);

    if(state)           //Clockwise rotation
    {   
        encoder->axis_position++;
        encoder->virtual_position == ELECTRIC_ROTATION_STEPS 
            ? encoder->virtual_position = 0 
            : encoder->virtual_position++;
    } 
    else if(!state)      //Counter-clockwise rotation
    {
        encoder->axis_position--;
        encoder->virtual_position == 0 
            ? encoder->virtual_position = ELECTRIC_ROTATION_STEPS 
            : encoder->virtual_position--;
    }
}


char QEncoder::get_pin_interrupt() const { return pin_interrupt; }
char QEncoder::get_pin_secondary() const { return pin_secondary;}

uint32_t QEncoder::get_axis_position() const { return axis_position;}
uint32_t QEncoder::get_virtual_position() const { return virtual_position; }