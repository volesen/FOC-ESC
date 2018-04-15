#pragma once

#define MECHANICAL_ROTATION_STEPS 2048	// 2048 steps per mechanical rotation
#define ELECTRIC_ROTATION_STEPS 342		// ceil(MECHANICAL_ROTATION_STEPS / 6)


#define NUM_MOTORS 2

enum motor_id : uint8_t
{
    motor0,
    motor1
};

struct pwm_phases
{
    public:
      /**
       * @brief Value of the three PWM phases.
       */
      float A, B, C;
    
      /**
       * @brief Creates a new pwm_phases. Phases can be initialized via constructor.
       */
      pwm_phases(const float &A = 0, const float &B = 0, const float &C = 0);

      /**
       * @brief Gets the phase with the largest magnitude.
       * 
       * @return Magnitude of phase with largest magnitude.
       */
      float get_max() const;

      /**
       * @brief Scales phase magnitudes so that the neutral point is at PWM_PERIOD / 2 and bound is at [0; PWM_PERIOD].
       * 
       * @remark Phases outside of [-bound_max; bound_max] results in undefined behavior. No protection because of speed requirements.
       * 
       * @param float bound_max : maximum magnitude to use as reference when scaling
       * 
       * @return A new pwm_phases with values scaled to valid pwm ranges.
       */
      pwm_phases create_corrected(const float &bound_max) const;

      /**
       * @brief Scales phase magnitudes so that the neutral point is at 0 and bound is at [-bound_max; bound_max].
       * 
       * @remark Phases outside of [0; PWM_PERIOD] results in undefined behavior. No protection because of speed requirements.
       * 
       * @param float bound_max : maximum magnitude to use as reference when scaling
       * 
       * @return A new pwm_phases with values scaled to be easy to manipulate.
       */
      pwm_phases create_decorrected(const float &bound_max) const;
};