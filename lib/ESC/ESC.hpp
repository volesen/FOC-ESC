#pragma once

#include "ADC_Motor.hpp"
#include "PID_Controller.hpp"

class ESC
{
  private:
    #pragma region Fields
    PID_Controller pid_torque, pid_waste;

    #pragma endregion

    #pragma region Initialization
    ESC();

    void initialize_classes();
    void reset_rotor_virtual_position(motor_id motor);
    

    #pragma endregion

  public:
    //Singleton necessities
    ESC(const ESC &) = delete;
    ESC &operator=(const ESC &) = delete;

    static ESC& get();
    static void initialize();

    void update();
};