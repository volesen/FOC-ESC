#include <iostream>
using namespace std;

#include "PID_Controller.cpp"

int main()
{
    PID_Controller troll (0.7, 0.6, 0.9, 30, 1,2);

    float error = 3;
    float ref = 93;

    for (int i = 0; i < 200; i++)
    {
        cout << error << "\n";
        error += troll.update(error, ref) + 6;
    }
    

    return 0;
}