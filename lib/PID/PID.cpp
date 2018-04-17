#include "PID.hpp"

//TODO:write more comments

pid_config::pid_config(double p, double i, double d, 
                       double i_max_change_per_cycle, 
                       uint32_t d_window_fast, uint32_t d_window_slow)
    : p(p), i(i), d(d)
    , i_max_change_per_cycle(i_max_change_per_cycle)
    , d_window_fast(d_window_fast), d_window_slow(d_window_slow) { }

//Constructor
PID_Controller::PID_Controller(pid_config config)
    : p(config.p), i(config.i), d(config.d)
    , p_term(0), i_term(0), d_term(0)
    , i_max_change_per_cycle(config.i_max_change_per_cycle)
    , error_sum_fast(0), error_sum_slow(0)
    , error_iteration(0)
{
    //TODO:We need to agree on how to handle errors
    //Do we guess and correct them? Or do we crash everything? I prefer crash.
    if (config.d_window_fast >= config.d_window_slow ||
        config.d_window_fast < 1 ||
        config.d_window_slow < 2)
    {
        error_rolling_window_fast = 1;
        error_rolling_window_slow = 2;
    }
    else
    {
        error_rolling_window_fast = config.d_window_fast;
        error_rolling_window_slow = config.d_window_slow;
    }

    errors = new double[error_rolling_window_slow];
    //There has to be a better way to initialize to 0
    for (uint32_t i = 0; i < error_rolling_window_slow; i++)  
        errors[i] = 0.0f;
}

//Destructor
PID_Controller::~PID_Controller()
{
    delete [] errors;
}

//Methods
double PID_Controller::update(const double &error)
{
    //Calculate p_term
    p_term = -error * p;

    //Calculate i_term
    i_term_previous = i_term;
    i_term -= error * i;

    //Mitigate integrator windup by limiting max change
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
    //-(Average fast error - average slow error) * d
    //Division operation is saved because the time step is one cycle
    d_term = -((error_sum_fast / error_rolling_window_fast) - (error_sum_slow / error_rolling_window_slow)) * d;

    //Return result
    return p_term + i_term + d_term;
}

double PID_Controller::update(const double &measurement, const double &reference)
{
    return update(measurement - reference);
}

pid_pair::pid_pair(pid_config waste, pid_config torque)
    : waste(waste), torque(torque) {}

pid_pair& PID::get(motor_id motor, pid_config waste, pid_config torque)
{
    //I do not feel like protecting against people deleting the object the pointer points to
    //This seems unnecessary given the circumstances the code is going to be used in.
    switch (motor)
    {
        case motor0:
        {   //The brackets are used to create a new scope so that the variable instance can be used multiple times
            static pid_pair instance(waste, torque);

            return instance;
        }
            
        case motor1:
        {
            static pid_pair instance(waste, torque);

            return instance;
        }

        default:
            //TODO: Throw an error or something
            break;
    }
}

void PID::initialize_all(pid_config waste, pid_config torque)
{
    for (uint8_t motor = 0; motor < NUM_MOTORS; motor++)
        get((motor_id)motor, waste, torque);
}