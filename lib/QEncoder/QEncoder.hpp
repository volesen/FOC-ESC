#pragma once

class QEncoder
{
    protected:
      QEncoder(char pin_interrupt, char pin_secondary, void (*interrupt_handler)());

      const char pin_interrupt;
      const char pin_secondary;

      uint32_t axis_position;
      uint32_t virtual_position;

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

      char get_pin_interrupt() const;
      char get_pin_secondary() const;

      uint32_t get_axis_position() const;
      uint32_t get_virtual_position() const;
};
