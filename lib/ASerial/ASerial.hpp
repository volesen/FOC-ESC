#pragma once

#include <stdint.h>
#include <atomic>
#include <Arduino.h>

#include "ESC_Globals.hpp"

class ASerial
{
  private:
    #pragma region Class instance
    //Singleton necessities
    ASerial();
    ASerial(const ASerial &){};
    ASerial &operator=(const ASerial &){};

    //Constructor
    ASerial(int baudrate);

    #pragma endregion

    #pragma region Fields
    volatile std::atomic<bool> _direction[NUM_MOTORS]; //true:positive CC, false:negative CW
    volatile std::atomic<uint16_t> _speed[NUM_MOTORS];
    volatile std::atomic<uint16_t> _position[NUM_MOTORS];

    uint8_t scope_buffer_data_available;
    uint8_t scope_buffer[3];

    bool _updated;

    #pragma endregion


    static void update_loop(void *input);

    #pragma region Helper methods
    static bool get_bit(uint8_t byte, uint8_t bit);
    static bool calculate_parity(uint8_t byte);
    
    void clear_scoped_buffer();

    #pragma endregion

    #pragma region Transmission error checking
        bool check_validity();

        bool check_ids();
        bool check_parity();
        bool check_lparity();

    #pragma endregion

    #pragma region Process Transmission
    /**
     * @brief Attempts to read from UART and update scope_buffer.
     * 
     * @return bool Whether or not buffer was updated with new data.
     */
    bool update_scope_buffer();

    void process_transmission();

    #pragma endregion

  public:
    static ASerial &get();
    static void initialize();

    #pragma region Properties
    bool ask_updated();

    float get_speed(motor_id motor);

    /**
     * @brief Updates the internal position field
     * 
     * @remark Position is not sent unless requested by master
     * 
     * @param uint16_t position Position to automatically send to controller.
     * 
     * @param motor_id motor Motor to update position of.
     * 
     */
    void update_position(uint16_t position, motor_id motor);

    #pragma endregion
};
