#pragma once

#include <stdint.h>

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
        PID_Controller(float p = 0, float i = 0, float d = 0, float i_max_change = 9999999, int32_t d_window_new = 1, int32_t d_window_old = 1);

        //Destructor
        ~PID_Controller();

        //Methods
        float update(const float &measurement, const float &reference);
};