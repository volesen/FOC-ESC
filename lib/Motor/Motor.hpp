#pragma once

struct motor_pwm
{
    public:
      /**
       * @brief Value of the three motor phases.
       */
      float A, B, C;
    
      /**
       * @brief Creates a new motor_pwm. Phases can be initialized via constructor.
       */
      motor_pwm(const float &A = 0, const float &B = 0, const float &C = 0);

      /**
       * @brief Gets the phase with the largest magnitude.
       * 
       * @return Magnitude of phase with largest magnitude.
       */
      float get_max() const;

      /**
       * @brief Scales phase magnitudes so that the neutral point is at PWM_PERIOD / 2 and bound is at [0; PWM_PERIOD].
       * 
       * @param float bound_max : maximum magnitude to use as reference when scaling
       * 
       * @return A new motor_pwm with values scaled to valid PWM ranges.
       */
      motor_pwm get_scaled(const float &bound_max) const;

      /**
       * @brief Scales phase magnitudes so that the neutral point is at 0 and bound is at [-bound_max; bound_max].
       * 
       * @param float bound_max : maximum magnitude to use as reference when scaling
       * 
       * @return A new motor_pwm with values scaled to be easy to manipulate.
       */
      motor_pwm get_descaled(const float &bound_max) const;
};

class Motor
{
    protected:
      //Fields
      /**
       * @brief motor_pwm struct that contains current scaled pwm values.
       */
      motor_pwm _pwm;

      /**
       * @brief The highest pwm phase magnitude passed to set_pwm.
       */
      float _pwm_max_bound;
      
      //Methods
      /**
       * @brief Stages the PWM output to be updated on next PWM period with values in _pwm struct.
       */
      virtual void update_pwm() = 0;

	public:
      //Properties
      /**
       * @brief The output pins of motor.
       * 
       * @return A pointer to a uint8_t[3] with three GPIO numbers representing the PWM output pins of motor.
       */
      virtual const uint8_t *get_pins() const = 0;

      /**
       * @brief Gets initialization status of motor.
       * 
       * @return A boolean value. true: motor initialized, false: motor not initialized.
       */
      virtual bool get_initialized() const = 0;

      /**
       * @brief Gets the highest pwm phase magnitude passed to set_pwm.
       * 
       * @return A float representing the highest pwm phase magnitude passed to set_pwm.
       */
      virtual float get_pwm_max_bound() const;

      /**
       * @brief Gets motor_pwm struct containing current scaled pwm values.
       * 
       * @return motor_pwm struct containing current scaled pwm values.
       */
      virtual motor_pwm get_pwm() const;

      /**
       * @brief Scale input parameter to PWM period values, save scaled parameter, and update PWM output to new values.
       * 
       * @param motor_pwm pwm : New PWM values to apply to motor.
       */
      virtual void set_pwm(const motor_pwm &pwm);

      /**
       * @brief Scale input parameters to PWM period values, save scaled parameters, and update PWM output to new values.
       * 
       * @param float A : New PWM value for phase A
       * @param float B : New PWM value for phase B
       * @param float C : New PWM value for phase C
       */
      virtual void set_pwm(const float &A, const float &B, const float &C);

      /**
       * @brief Set all PWM outputs to ground.
       */
      virtual void set_pwm_low();

      /**
       * @brief Set all PWM outputs to 3.3 V.
       */
      virtual void set_pwm_high();

      //Methods
      /**
       * @brief Gets the singleton instance of a motor.
       * 
       * @param char id : The ID of the motor to get.
       * 
       * @return Singleton instance of a motor.
       */
      static Motor& get(char id);

      /**
       * @brief Initializes all Motor0 and Motor1.
       */
      static void initialize_all();

      /**
       * @brief Initialize motor by setting up PWM controls.
       */
      virtual Motor& initialize() = 0;
      
};

class Motor0 : public Motor
{
    protected:
      //Singleton necessities
      Motor0();
      Motor0(const Motor0 &){};
      Motor0 &operator=(const Motor0 &){};
      
      //Fields
      /**
       * @brief Initialization status of Motor0. 
       */
      static bool _initialized;
      
      /**
       * @brief Output pins of Motor0.
       */
      uint8_t _pins[3];
      
      //Methods
      /**
       * @brief Stages the PWM output to be updated on next PWM period with values in _pwm struct.
       */
      void update_pwm();
        
    public:
      //Properties
      /**
       * @brief The output pins of Motor0.
       * 
       * @return A pointer to a uint8_t[3] with three GPIO numbers representing the PWM output pins of Motor0.
       */
      const uint8_t *get_pins() const;

      /**
       * @brief Gets initialization status of Motor0.
       * 
       * @return A boolean value. true: Motor0 initialized, false: Motor0 not initialized.
       */
      bool get_initialized() const;

      //Methods
      /**
       * @brief Gets the singleton instance of Motor0.
       * 
       * @return Singleton instance of Motor0.
       */
      static Motor0 &get();

      /**
       * @brief Initialize motor by setting up PWM controls.
       */
      Motor0& initialize();
};

class Motor1 : public Motor
{
    protected:
      //Singleton necessities
      Motor1();
      Motor1(const Motor1 &){};
      Motor1 &operator=(const Motor1 &){};
      
      //Fields
      /**
       * @brief Initialization status of Motor1. 
       */
      static bool _initialized;
      
      /**
       * @brief Output pins of Motor1.
       */
      uint8_t _pins[3];
      
      //Methods
      /**
       * @brief Stages the PWM output to be updated on next PWM period with values in _pwm struct.
       */
      void update_pwm();
        
    public:
      //Properties
      /**
       * @brief The output pins of Motor1.
       * 
       * @return A pointer to a uint8_t[3] with three GPIO numbers representing the PWM output pins of Motor1.
       */
      const uint8_t *get_pins() const;

      /**
       * @brief Gets initialization status of Motor1.
       * 
       * @return A boolean value. true: Motor1 initialized, false: Motor1 not initialized.
       */
      bool get_initialized() const;

      //Methods
      /**
       * @brief Gets the singleton instance of Motor1.
       * 
       * @return Singleton instance of Motor1.
       */
      static Motor1 &get();

      /**
       * @brief Initialize motor by setting up PWM controls.
       */
      Motor1& initialize();
};
