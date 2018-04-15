#pragma once

#include <stdint.h>
#include "ESC_Globals.hpp"

//TODO: write documentation

struct pid_pair
{
    pid_pair(pid_config waste = pid_config(), 
             pid_config torque = pid_config());

    PID_Controller waste;
    PID_Controller torque;
};

class PID
{
    public:
        //Sorry, this is bad...
        //The get+initialize pattern is contagious... too late now
        static pid_pair& get(motor_id motor, 
                             pid_config waste = pid_config(), 
                             pid_config torque = pid_config());
        static void initialize_all(pid_config waste = pid_config(), 
                                   pid_config torque = pid_config());
};

struct pid_config
{
    pid_config(float p = 0, float i = 0, float d = 0, 
               float i_max_change = 9999999, 
               uint32_t d_window_fast = 1, uint32_t d_window_slow = 2);

    float p, i, d, i_max_change_per_cycle;
    uint32_t d_window_fast, d_window_slow;
};

class PID_Controller
{
    private:
        float p_term;
        float i_term;
        float d_term;

        float i_term_previous;
        float i_max_change_per_cycle;

        float error_sum_fast, error_sum_slow;
        int32_t error_iteration;
        int32_t error_rolling_window_fast, error_rolling_window_slow;
        float *errors;

      public:
        //Fields
        float p, i, d;

        //Constructor
        PID_Controller(pid_config config);
        //PID_Controller(float p = 0, float i = 0, float d = 0, float i_max_change = 9999999, int32_t d_window_fast = 1, int32_t d_window_slow = 2);

        //Destructor
        ~PID_Controller();

        //Methods
        float update(const float &measurement, const float &reference);
};