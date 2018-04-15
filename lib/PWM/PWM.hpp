#pragma once

#include <stdint.h>
#include "ESC_Globals.hpp"

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

class PWM
{
    protected:
      //Fields
      /**
       * @brief pwm_phases struct that contains current scaled pwm values.
       */
      pwm_phases _pwm;

      /**
       * @brief The highest pwm phase magnitude passed to set_pwm.
       */
      float _pwm_max_bound;
      
      //Methods
      /**
       * @brief Stages the pwm output to be updated on next pwm period with values in _pwm struct.
       */
      virtual void update_pwm() = 0;

	public:
      //Properties
      /**
       * @brief The output pins of PWM.
       * 
       * @return A pointer to a uint8_t[3] with three GPIO numbers representing the PWM output pins of PWM.
       */
      virtual const uint8_t *get_pins() const = 0;

      /**
       * @brief Gets initialization status of PWM.
       * 
       * @return A boolean value. true: PWM initialized, false: PWM not initialized.
       */
      virtual bool get_initialized() const = 0;

      /**
       * @brief Gets the highest pwm phase magnitude passed to set_pwm.
       * 
       * @return A float representing the highest pwm phase magnitude passed to set_pwm.
       */
      virtual float get_phases_max_bound() const;

      /**
       * @brief Gets pwm_phases struct containing decorrected pwm values.
       * 
       * @return pwm_phases struct containing decorrected pwm values.
       */
      virtual pwm_phases get_phases() const;

      /**
       * @brief Correct/scale input parameter to pwm period values, save corrected parameter, and update PWM output to new values.
       * 
       * @param pwm_phases pwm : New pwm values to apply to PWM.
       */
      virtual void set_pwm(const pwm_phases &pwm);

      /**
       * @brief Correct/scale input parameters to pwm period values, save corrected parameters, and update PWM output to new values.
       * 
       * @param float A : New pwm value for phase A
       * @param float B : New pwm value for phase B
       * @param float C : New pwm value for phase C
       */
      virtual void set_pwm(const float &A, const float &B, const float &C);

      /**
       * @brief Set all PWM outputs to ground.
       * 
       * @param bool A : If phase A should be set low
       * @param bool B : If phase B should be set low
       * @param bool C : If phase C should be set low
       */
      virtual void set_pwm_low(bool A = true, bool B = true, bool C = true);

      /**
       * @brief Set all PWM outputs to 3.3 V.
       * 
       * @param bool A : If phase A should be set high
       * @param bool B : If phase B should be set high
       * @param bool C : If phase C should be set high
       */
      virtual void set_pwm_high(bool A = true, bool B = true, bool C = true);

      //Methods
      /**
       * @brief Gets the singleton instance of a PWM.
       * 
       * @param motor_id motor : The PWM module tied to motor_id to get.
       * 
       * @return Singleton instance of a PWM.
       */
      static PWM& get(motor_id motor);

      /**
       * @brief Initializes all PWM0 and PWM1.
       * 
       * @param float initial_pwm_max_bound : Initial value for pwm_max_bound
       */
      static void initialize_all(float initial_pwm_max_bound = 0);

      /**
       * @brief Initialize pwm output by setting up PWM controls.
       */
      virtual PWM& initialize(float initial_pwm_max_bound = 0) = 0;
      
};

class PWM0 : public PWM
{
    protected:
      //Singleton necessities
      PWM0();
      PWM0(const PWM0 &){};
      PWM0 &operator=(const PWM0 &){};
      
      //Fields
      /**
       * @brief Initialization status of PWM0. 
       */
      static bool _initialized;
      
      /**
       * @brief Output pins of PWM0.
       */
      uint8_t _pins[3];
      
      //Methods
      /**
       * @brief Stages the PWM output to be updated on next pwm period with values in _pwm struct.
       */
      void update_pwm();
        
    public:
      //Properties
      /**
       * @brief The output pins of PWM0.
       * 
       * @return A pointer to a uint8_t[3] with three GPIO numbers representing the pwm output pins of PWM0.
       */
      const uint8_t *get_pins() const;

      /**
       * @brief Gets initialization status of PWM0.
       * 
       * @return A boolean value. true: PWM0 initialized, false: PWM0 not initialized.
       */
      bool get_initialized() const;

      //Methods
      /**
       * @brief Gets the singleton instance of PWM0.
       * 
       * @return Singleton instance of PWM0.
       */
      static PWM0 &get();

      /**
       * @brief Initialize pwm output by setting up PWM controls.
       * 
       * @param float initial_pwm_max_bound : Initial value for pwm_max_bound
       */
      PWM0& initialize(float initial_pwm_max_bound = 0);
};

class PWM1 : public PWM
{
    protected:
      //Singleton necessities
      PWM1();
      PWM1(const PWM1 &){};
      PWM1 &operator=(const PWM1 &){};
      
      //Fields
      /**
       * @brief Initialization status of PWM1. 
       */
      static bool _initialized;
      
      /**
       * @brief Output pins of PWM1.
       */
      uint8_t _pins[3];
      
      //Methods
      /**
       * @brief Stages the PWM output to be updated on next pwm period with values in _pwm struct.
       */
      void update_pwm();
        
    public:
      //Properties
      /**
       * @brief The output pins of PWM1.
       * 
       * @return A pointer to a uint8_t[3] with three GPIO numbers representing the pwm output pins of PWM1.
       */
      const uint8_t *get_pins() const;

      /**
       * @brief Gets initialization status of PWM1.
       * 
       * @return A boolean value. true: PWM1 initialized, false: PWM1 not initialized.
       */
      bool get_initialized() const;

      //Methods
      /**
       * @brief Gets the singleton instance of PWM1.
       * 
       * @return Singleton instance of PWM1.
       */
      static PWM1 &get();

      /**
       * @brief Initialize pwm output by setting up PWM controls.
       * 
       * @param float initial_pwm_max_bound : Initial value for pwm_max_bound
       */
      PWM1& initialize(float initial_pwm_max_bound = 0);
};
