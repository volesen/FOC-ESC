#pragma once

#include "ESC_Globals.hpp"

class ESC
{
  private:
    #pragma region Fields
    uint32_t target_position[NUM_MOTORS];

    #pragma endregion

    static double calculate_position_error(uint32_t const &measurement, uint32_t const &reference, bool direction);

    #pragma region Initialization
    ESC();

    void initialize_classes();
    void reset_rotor_virtual_position(motor_id motor);
    void reset_axis_position(motor_id motor);

    #pragma endregion

  public:
    //Singleton necessities
    ESC(const ESC &) = delete;
    ESC &operator=(const ESC &) = delete;

    static ESC& get();
    static void initialize();

    void update();
};