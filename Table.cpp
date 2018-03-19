#define ELECTRIC_ROTATION_STEPS 342		// ceil(1/6 * ELECTRIC_ROTATION_STEPS)
#define PI 3.14159265358979323846

#include <stdio.h>
#include <math.h>

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float sin_table[ELECTRIC_ROTATION_STEPS] = {0};

int main()
{
    for(int i = 0; i < ELECTRIC_ROTATION_STEPS; i++) {
        float n = map(i, 0, ELECTRIC_ROTATION_STEPS, 0, 2*PI);
        printf("%.32ff\n", sinf(n));
    }
    
    return 0;
}