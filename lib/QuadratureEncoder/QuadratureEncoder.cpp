#include "QuadratureEncoder.hpp"
#include "Transform.hpp"

QuadratureEncoder::QuadratureEncoder(char pin_interrupt, char pin_secondary)
    : pin_interrupt(pin_interrupt), pin_secondary(pin_secondary), 
      axis_position(0), virtual_position(0)
{
    pinMode(pin_interrupt, INPUT_PULLUP); //TODO:Test if pull down works instead
    pinMode(pin_secondary, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_interrupt),this->handle_interrupt, RISING); //Trigger interrupt on pin 23, on a rising edge
}

QuadratureEncoder::~QuadratureEncoder()
{
    detachInterrupt(digitalPinToInterrupt(pin_interrupt));
}

void IRAM_ATTR QuadratureEncoder::handle_interrupt()
{
    
    bool state = digitalRead(pin_secondary);

    if(state)   //Clockwise rotation
    {   
        axis_position++;
        virtual_position = virtual_position == ELECTRIC_ROTATION_STEPS ? 0 : virtual_position + 1;
    } 
    else if(state)  //Counter Clockwise rotation
    {
        axis_position--;
        virtual_position = virtual_position == 0 ? ELECTRIC_ROTATION_STEPS : virtual_position - 1;
    }
}

char QuadratureEncoder::get_pin_interrupt() const { return pin_interrupt; }
char QuadratureEncoder::get_pin_secondary() const { return pin_secondary;}

uint32_t QuadratureEncoder::get_axis_position() const { return axis_position;}
uint32_t QuadratureEncoder::get_virtual_position() const { return virtual_position; }