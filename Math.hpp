#pragma once

typedef float float32_t;
typedef int uint16_t;

#include "tables.hpp"

#define MECHICAL_ROTATION_STEPS 2048	// 2048 steps per mechanical rotation
#define ELECTRIC_ROTATION_STEPS 342		// ceil(1/6 * ELECTRIC_ROTATION_STEPS)

class Math {
    public:
        static float32_t idx(uint16_t n);
        static float32_t sin(uint16_t theta);
        static float32_t cos(uint16_t theta);
};