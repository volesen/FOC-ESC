#pragma once

#include <stdint.h>
#include <atomic>

#include "ESC_Globals.hpp"

class ESC_Serial
{
  private:
    #pragma region Class instance
    //Singleton necessities
    ESC_Serial();
    ESC_Serial(const ESC_Serial &){};
    ESC_Serial &operator=(const ESC_Serial &){};

    //Constructor
    ESC_Serial(int baudrate);

    #pragma endregion

    #pragma region Fields
    volatile std::atomic<bool> _direction[NUM_MOTORS]; //true:positive CC, false:negative CW
    volatile std::atomic<uint16_t> _throttle[NUM_MOTORS];
    volatile std::atomic<uint32_t> _position[NUM_MOTORS];

    uint8_t scope_buffer_data_available;
    uint8_t scope_buffer[3];

    volatile std::atomic<bool> _updated[NUM_MOTORS];

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
    static ESC_Serial &get();
    static void initialize();

    #pragma region Properties
    bool ask_updated(motor_id motor);

    float get_throttle(motor_id motor);

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
    void update_position(motor_id motor, uint32_t position);

    #pragma endregion
};
