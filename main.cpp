#include <iostream>
#include <stdint.h>

using namespace std;

#include "PID_Controller.hpp"

int32_t main()
{
    PID_Controller troll (0.7, 0.6, 0.9, 30, 1,2);

    float32_t error = 3;
    float32_t ref = 93;

    for (int32_t i = 0; i < 200; i++)
    {
        cout << error << "\n";
        error += troll.update(error, ref) + 6;
    }
    

    return 0;
}