#include "PID.hpp"

//Constructor
PID_Controller::PID_Controller(float32_t p, float32_t i, float32_t d, float32_t i_max_change_per_cycle, int32_t d_window_fast, int32_t d_window_slow)
    : p(p), i(i), d(d)
    , p_term(0), i_term(0), d_term(0)
    , i_max_change_per_cycle(i_max_change_per_cycle)
    , error_sum_fast(0), error_sum_slow(0)
    , error_iteration(0)
{
    //TODO:We need to agree on how to handle errors
    //Do we guess and correct them? Or do we crash everything? I prefer crash.
    if (d_window_fast >= d_window_slow ||
        d_window_fast < 1 ||
        d_window_slow < 2)
    {
        error_rolling_window_fast = 1;
        error_rolling_window_slow = 2;
    }
    else
    {
        error_rolling_window_fast = d_window_fast;
        error_rolling_window_slow = d_window_slow;
    }

    errors = new float32_t[error_rolling_window_slow];
    //There has to be a better way to initialize to 0
    for (int32_t i = 0; i < error_rolling_window_slow; i++)  
        errors[i] = 0.0f;
}

//Destructor
PID_Controller::~PID_Controller()
{
    delete [] errors;
}

//Methods
float32_t PID_Controller::update(const float32_t &measurement, const float32_t &reference)
{
    float32_t error = measurement - reference;
    
    //Calculate p_term
    p_term = -error * p;

    //Calculate i_term
    i_term_previous = i_term;
    i_term -= error * i;

    //Mitigate integrator windup
    if (i_term - i_term_previous > i_max_change_per_cycle)
        i_term = i_term_previous + i_max_change_per_cycle;
    else if (i_term - i_term_previous < -i_max_change_per_cycle)
        i_term = i_term_previous - i_max_change_per_cycle;

    //Calculating d_term (with moving averages)
    //The rolling windows will not be filled at the start
    //This will cause the d_term contribution to be small
    
    //Update new error window sum
    int32_t error_iteration_limit_new = error_iteration - error_rolling_window_fast < 0 
                                        ? error_rolling_window_slow + error_iteration - error_rolling_window_fast
                                        : error_iteration - error_rolling_window_fast;

    error_sum_fast += error - errors[error_iteration_limit_new];
    error_sum_slow += error - errors[error_iteration];
    
    errors[error_iteration] = error;

    if (++error_iteration == error_rolling_window_slow)
        error_iteration = 0;

    //Calculate d_term
    d_term = -((error_sum_fast / error_rolling_window_fast) - (error_sum_slow / error_rolling_window_slow)) / 2 * d;

    //Return result
    return p_term + i_term + d_term;
}