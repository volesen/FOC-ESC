#pragma once

#include <Arduino.h>

class QuadratureEncoder
{
    protected:
      const char pin_interrupt;
      const char pin_secondary;

      uint32_t axis_position;
      uint32_t virtual_position;

      void IRAM_ATTR handle_interrupt();

    public:
      QuadratureEncoder(char pin_interrupt, char pin_secondary);
      ~QuadratureEncoder();

      char get_pin_interrupt() const;
      char get_pin_secondary() const;

      uint32_t get_axis_position() const;
      uint32_t get_virtual_position() const;
      
}
