#pragma once

#include <Arduino.h>

enum class QuadratureEncoderSlot
{
  Encoder0,
  Encoder1
};

class QuadratureEncoder
{
    protected:
      static QuadratureEncoder* Encoder0;
      static QuadratureEncoder* Encoder1;

      const char pin_interrupt;
      const char pin_secondary;
      const QuadratureEncoderSlot encoder_slot;

      uint32_t axis_position;
      uint32_t virtual_position;

      static void IRAM_ATTR handle_interrupt(QuadratureEncoder* const Encoder);
      static void IRAM_ATTR handle_interrupt_Encoder0();
      static void IRAM_ATTR handle_interrupt_Encoder1();

    public:
      QuadratureEncoder(char pin_interrupt, char pin_secondary, QuadratureEncoderSlot slot);
      ~QuadratureEncoder();

      char get_pin_interrupt() const;
      char get_pin_secondary() const;

      uint32_t get_axis_position() const;
      uint32_t get_virtual_position() const;
};
