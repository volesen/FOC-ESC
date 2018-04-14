#pragma once

#include <stdint.h>
#include <atomic>

#include <Arduino.h>

#include "ESC_Globals.hpp"

#define TX_PIN 1
#define RX_PIN 3

#define MAX_INPUT_SPEED 4095.0
#define MAX_OUTPUT_SPEED 1.00

#define ASerial_BAUDRATE 9600       //TODO:use 921600 instead

#pragma region Bit selectors
//Most significant bit is leftmost
//Bit 7 is the most significant bit

//Master packet:
//ID  Parity  Linked parity   Position request  Direction   Motor ID
//{1} {1}     {2}             {1}               {1}         {2}
//Data packets
//ID  Parity  Speed
//{1} {1}     {6}

#define GET_ID_BIT(byte) get_bit(byte, 7)
#define GET_PARITY_BIT(byte) get_bit(byte, 6)

#define GET_LPARITY_BIT(byte, id_data) get_bit(byte, 5 - id_data) //id_data \in [0, 1]
#define GET_POS_REQ_BIT(byte) get_bit(byte, 3)
#define GET_DIRECTION_BIT(byte) get_bit(byte, 2)
#define GET_MOTOR_ID_BITS(byte) byte & 3 //Mask away everything but the two least significant bits
#define GET_SPEED_BITS(byte) byte & 63   //Mask away the two most significant bits

#pragma endregion

enum class motor_id
{
    motor0,
    motor1
};

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
};
