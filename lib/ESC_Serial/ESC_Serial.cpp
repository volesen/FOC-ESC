#include <Arduino.h>
#include <esp_task_wdt.h>

#include "ESC_Serial.hpp"

#define TX_PIN 1
#define RX_PIN 3

#define MAX_INPUT_THROTTLE 4095.0
#define MAX_OUTPUT_THROTTLE 1.00

#define ESC_Serial_BAUDRATE 921600

#pragma region Bit selectors
//Send order:
//0. data0, 1. data1, 2. master

//scope_buffer order:
//0. master, 1. data1, 2. data0

#define PDATA0 2
#define PDATA1 1
#define PMASTER 0

//Most significant bit is leftmost
//Bit 7 is the most significant bit

//Master packet:
//ID  Parity  Linked parity   Position request  Direction   Motor ID
//{1} {1}     {2}             {1}               {1}         {2}
//Data packets
//ID  Parity  Throttle
//{1} {1}     {6}

#define GET_ID_BIT(byte) get_bit(byte, 7)
#define GET_PARITY_BIT(byte) get_bit(byte, 6)

#define GET_LPARITY_BIT(byte, id_data) get_bit(byte, 5 - id_data) //id_data \in [0, 1]
#define GET_POS_REQ_BIT(byte) get_bit(byte, 3)
#define GET_DIRECTION_BIT(byte) get_bit(byte, 2)
#define GET_MOTOR_ID_BITS(byte) (byte & 3) //Mask away everything but the two least significant bits
#define GET_THROTTLE_BITS(byte) (byte & 63)   //Mask away the two most significant bits

#pragma endregion


void ESC_Serial::update_loop(void *input)
{
    //Ideally this should never be needed, but it is here in case there is a bug, 
    //so that everything doesn't just hang
    Serial.setTimeout(8);

    Serial.begin(ESC_Serial_BAUDRATE);

    while (true)
    {
        //I think this allows other tasks to run on CPU 0
        yield();

        //If no update to scope_buffer or there are not three packets, continue
        //If scope_buffer was updated, check scope_buffer for valid transmission,
        //continue if no valid transmission
        if (!get().update_scope_buffer() ||
            !get().check_validity())
            continue;

        //If the code makes it down here, the transmission is valid
        //Process the transmission and apply changes
        get().process_transmission();

        // ATTENTION: Uncomment if Arduino framework is prevented from running
        // vTaskDelay(1 / portTICK_PERIOD_MS);
    }    
}



#pragma region Helper methods
bool ESC_Serial::get_bit(uint8_t byte, uint8_t bit)
{
    //AND with single bit mask
    //Check if over 0
    return (byte & (1 << bit)) > 0;
}

bool ESC_Serial::calculate_parity(uint8_t byte)
{
    // CREDIT:
    // By Sean Eron Anderson
    // seander@cs.stanford.edu
    // https://graphics.stanford.edu/~seander/bithacks.html#ParityParallel
    // 1 if odd number of bits set
    // 0 if even number of bits set
    byte ^= byte >> 4;
    byte &= 0xf;
    return (0x6996 >> byte) & 1 == 1;
}


void ESC_Serial::clear_scoped_buffer()
{
    scope_buffer_data_available = 0;
}

#pragma endregion

#pragma region Transmission error checking
bool ESC_Serial::check_validity()
{
    return (check_ids() &&
            check_parity() &&
            check_lparity());
}


bool ESC_Serial::check_ids()
{
    return (!GET_ID_BIT(scope_buffer[PDATA0]) &&
            GET_ID_BIT(scope_buffer[PDATA1]) &&
            GET_ID_BIT(scope_buffer[PMASTER]));
}

bool ESC_Serial::check_parity()
{
    return (!calculate_parity(scope_buffer[PDATA0]) &&
            !calculate_parity(scope_buffer[PDATA1]) &&
            calculate_parity(scope_buffer[PMASTER]));
}

bool ESC_Serial::check_lparity()
{
    return (GET_PARITY_BIT(scope_buffer[PDATA0]) == GET_LPARITY_BIT(scope_buffer[PMASTER], 0) &&
            GET_PARITY_BIT(scope_buffer[PDATA1]) == GET_LPARITY_BIT(scope_buffer[PMASTER], 1));
}

#pragma endregion

#pragma region Process Transmission
bool ESC_Serial::update_scope_buffer()
{   
    //We are not going to do the cyclic efficient array storrage
    //that we did in the PID loop.
    //We are only working with 3 uint8_t here so the complexity is unnecessary.
    uint32_t rx_buffer_available = Serial.available();

    //No change if there is nothing to read
    if (rx_buffer_available == 0)
        return false;
    
    //Initialize num_bytes_to_read with if statements
    uint8_t num_bytes_to_read;
    //If scope_buffer already full (previous transmission invalid)
    if (scope_buffer_data_available == 3)
    {
        num_bytes_to_read = 1;
        scope_buffer_data_available--;
    }
    //If enough data in rx buffer to fill out scope_buffer
    else if (rx_buffer_available >= 3 - scope_buffer_data_available)
        num_bytes_to_read = 3 - scope_buffer_data_available;
    //Else not enough data in rx buffer to fill out scope_buffer
    else
        num_bytes_to_read = rx_buffer_available;
        

    //Shift scope_buffer to make room for read
    if (num_bytes_to_read == 1)
    {
        scope_buffer[2] = scope_buffer[1];
        scope_buffer[1] = scope_buffer[0];
    }
    else if (num_bytes_to_read == 2)
        scope_buffer[2] = scope_buffer[0];
    
    //Read bytes to scope_buffer
    for (int i = num_bytes_to_read - 1; i > -1; i--)    
        scope_buffer[i] = Serial.read();

    //Update size of scope_buffer array
    scope_buffer_data_available += num_bytes_to_read;

    //Return true if there are three packets to read
    return scope_buffer_data_available == 3;
}

void ESC_Serial::process_transmission()
{
    motor_id target_motor = GET_MOTOR_ID_BITS(scope_buffer[PMASTER]) < NUM_MOTORS 
                            ? (motor_id)GET_MOTOR_ID_BITS(scope_buffer[PMASTER])
                            : motor0;        //TODO: Create error here

    _direction[target_motor] = GET_DIRECTION_BIT(scope_buffer[PMASTER]);

    //First data packet contains the lower order throttle
    //Second data packet contains the higher order throttle
    _throttle[target_motor] = GET_THROTTLE_BITS(scope_buffer[PDATA0]) |
                             (GET_THROTTLE_BITS(scope_buffer[PDATA1]) << 6);

    //Set update status so external code knows.
    //All get properties have been updated, so get_throttle is ready now.
    _updated[target_motor] = true;

    //If controller requests current position
    if (GET_POS_REQ_BIT(scope_buffer[PMASTER]))
    {
        //Copy position to avoid two atomic checks
        //and to avoid a block if position is updated inbetween serial byte writes
        uint32_t position = _position[target_motor];
        //Put current position into tx buffer
        //Least significant bits are written first
        Serial.write((uint8_t *)&position, 4);
        //Block and force tx buffer to send
        Serial.flush();
    }

    //Clear scoped_buffer because transmission has been processed.
    //This opens up room for the next transmission.
    clear_scoped_buffer();
}

#pragma endregion 


ESC_Serial& ESC_Serial::get()
{
    static ESC_Serial com(ESC_Serial_BAUDRATE);

    return com;
}

void ESC_Serial::initialize()
{
    get();
}

ESC_Serial::ESC_Serial(int baudrate)
{
    //Initialize fields
    scope_buffer_data_available = 0;
    for (int i = 0; i < 3; i++)
        scope_buffer[i] = 0;

    for (int i = 0; i < NUM_MOTORS; i++)
    {
        std::atomic_init(_direction + i, true);
        _throttle[i] = 0;
        _position[i] = 0;

        std::atomic_init(_updated + i, false);
    }
    
    //The update loop is given priority 0 which is the lowest priority.
    //We might hog all resources from the Arduino framework if we raise the priority.
    xTaskCreatePinnedToCore(update_loop, "update_loop", 4 * 1024, NULL, 0, NULL, 0);
    //Unsubscribe from task watchdog as apparently I can't feed it correctly
    esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0));
}

#pragma region Properties
bool ESC_Serial::ask_updated(motor_id motor)
{
    return _updated[motor];
}

float ESC_Serial::get_throttle(motor_id motor)
{
    //Copied to minimize chance of being blocked by accessing atomic variables
    //which creates two opportunities for a block
    float throttle = (float)_throttle[motor] 
                * (_direction[motor] ? 1 : -1); //Make negative if CC direction

    if (_updated[motor])
        _updated[motor] = false;

    return throttle / MAX_INPUT_THROTTLE * MAX_OUTPUT_THROTTLE;
}

void ESC_Serial::update_position(motor_id motor, uint32_t position)
{
    _position[motor] = position;
}

#pragma endregion
