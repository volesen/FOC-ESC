#pragma once

#include <stdint.h>
#include "Tables.hpp"

#define MECHICAL_ROTATION_STEPS 2048	// 2048 steps per mechanical rotation
#define ELECTRIC_ROTATION_STEPS 342		// ceil(1/6 * ELECTRIC_ROTATION_STEPS)

class Trig {
    public:
        static float idx(uint16_t n);
        static float sin(uint16_t theta);
        static float cos(uint16_t theta);
};