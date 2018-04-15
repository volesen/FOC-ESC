#pragma once

#include <atomic>

class QEncoder
{
    protected:
      QEncoder(char pin_interrupt, char pin_secondary, void (*interrupt_handler)());

      const char pin_interrupt;
      const char pin_secondary;

      volatile std::atomic<uint32_t> axis_position;
      volatile std::atomic<uint32_t> virtual_position;

      static void IRAM_ATTR handle_interrupt_encoder0();
      static void IRAM_ATTR handle_interrupt_encoder1();
      static void IRAM_ATTR handle_interrupt(QEncoder* const encoder);

    public:
      QEncoder() = delete;
      QEncoder(const QEncoder &encoder) = delete;
      QEncoder &operator=(const QEncoder &encoder) = delete;
      
      ~QEncoder();

      static void initialize_all();
      static QEncoder& get(char id);

      void reset_virtual_position();
      void reset_axis_position();

      char get_pin_interrupt() const;
      char get_pin_secondary() const;

      uint32_t get_axis_position() const;
      uint32_t get_virtual_position() const;
};
