#pragma once

#include <stdint.h>

class PID_Controller 
{
    private:
        float32_t p_term;
        float32_t i_term;
        float32_t d_term;

        float32_t i_term_previous;
        float32_t i_max_change_per_cycle;

        float32_t error_sum_new, error_sum_old;
        int32_t error_rolling_window_fast, error_rolling_window_slow;
        int32_t error_iteration;
        float32_t *errors;

      public:
        //Fields
        float32_t p, i, d;

        //Constructor
        PID_Controller(float32_t P = 0, float32_t I = 0, float32_t D = 0, float32_t i_max_change = 9999999, int32_t d_window_new = 1, int32_t d_window_old = 1);

        //Destructor
        ~PID_Controller();

        //Methods
        float32_t update(float32_t measurement, float32_t reference);
};