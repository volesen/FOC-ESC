#pragma once

#include <stdint.h>
#include "ESC_Globals.hpp"

//TODO: write documentation

struct pid_config
{
    pid_config(double p = 0, double i = 0, double d = 0, 
               double i_max_change = 9999999, 
               uint32_t d_window_fast = 1, uint32_t d_window_slow = 2);

    double p, i, d, i_max_change_per_cycle;
    uint32_t d_window_fast, d_window_slow;
};

class PID_Controller
{
    private:
        double p_term;
        double i_term;
        double d_term;

        double i_term_previous;
        double i_max_change_per_cycle;

        double error_sum_fast, error_sum_slow;
        int32_t error_iteration;
        int32_t error_rolling_window_fast, error_rolling_window_slow;
        double *errors;

      public:
        //Fields
        double p, i, d;

        //Constructor
        PID_Controller(pid_config config);
        //PID_Controller(double p = 0, double i = 0, double d = 0, double i_max_change = 9999999, int32_t d_window_fast = 1, int32_t d_window_slow = 2);

        //Destructor
        ~PID_Controller();

        //Methods
        double update(const double &error);
        double update(const double &measurement, const double &reference);
};

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