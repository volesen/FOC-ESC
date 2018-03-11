#ifndef PID_Controller_H
#define PID_Controller_H

class PID_Controller 
{
    private:
        float p_term;
        float i_term;
        float d_term;

        float i_term_previous;
        float i_max_change_per_cycle;

        float error_sum_new, error_sum_old;
        int error_rolling_window_fast, error_rolling_window_slow;
        int error_iteration;
        float *errors;

      public:
        //Fields
        float p, i, d;

        //Constructor
        PID_Controller(float P = 0, float I = 0, float D = 0, float i_max_change = 9999999, int d_window_new = 1, int d_window_old = 1);

        //Destructor
        ~PID_Controller();

        //Methods
        float update(float measurement, float reference);
};

#endif