#include "QuadratureEncoder.hpp"
#include "Transform.hpp"

QuadratureEncoder *QuadratureEncoder::Encoder0 = 0;
QuadratureEncoder *QuadratureEncoder::Encoder1 = 0;

QuadratureEncoder::QuadratureEncoder(char pin_interrupt, char pin_secondary, QuadratureEncoderSlot encoder_slot)
    : pin_interrupt(pin_interrupt), pin_secondary(pin_secondary), 
      axis_position(0), virtual_position(0), encoder_slot(encoder_slot)
{
    pinMode(pin_interrupt, INPUT_PULLUP); //TODO:Test if pull down works instead
    pinMode(pin_secondary, INPUT_PULLUP);

    void (*interrupt_handler)();

    switch (encoder_slot)
    {
        case QuadratureEncoderSlot::Encoder0:
            Encoder0 = this;
            interrupt_handler = handle_interrupt_Encoder0;

            break;

        case QuadratureEncoderSlot::Encoder1:
            Encoder1 = this;
            interrupt_handler = handle_interrupt_Encoder0;

            break;

        default:
            break;
    }

    attachInterrupt(digitalPinToInterrupt(pin_interrupt), interrupt_handler, 1);
}

QuadratureEncoder::~QuadratureEncoder()
{
    detachInterrupt(digitalPinToInterrupt(pin_interrupt));

    switch (encoder_slot)
    {
        case QuadratureEncoderSlot::Encoder0:
            Encoder0 = 0;

            break;

        case QuadratureEncoderSlot::Encoder1:
            Encoder1 = 0;

            break;

        default:
            break;
    }
}

void IRAM_ATTR QuadratureEncoder::handle_interrupt(QuadratureEncoder* const encoder)
{
    
    bool state = digitalRead(encoder->pin_secondary);

    if(state)   //Clockwise rotation
    {   
        encoder->axis_position++;
        encoder->virtual_position = encoder->virtual_position == ELECTRIC_ROTATION_STEPS ? 0 : encoder->virtual_position + 1;
    } 
    else if(state)  //Counter Clockwise rotation
    {
        encoder->axis_position--;
        encoder->virtual_position = encoder->virtual_position == 0 ? ELECTRIC_ROTATION_STEPS : encoder->virtual_position - 1;
    }
}

void IRAM_ATTR QuadratureEncoder::handle_interrupt_Encoder0()
{
    handle_interrupt(Encoder0);
}

void IRAM_ATTR QuadratureEncoder::handle_interrupt_Encoder1()
{
    handle_interrupt(Encoder1);
}

char QuadratureEncoder::get_pin_interrupt() const { return pin_interrupt; }
char QuadratureEncoder::get_pin_secondary() const { return pin_secondary;}

uint32_t QuadratureEncoder::get_axis_position() const { return axis_position;}
uint32_t QuadratureEncoder::get_virtual_position() const { return virtual_position; }