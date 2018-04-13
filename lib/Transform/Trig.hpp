#pragma once

#include <stdint.h>
#include "Tables.hpp"
#include "ESC_Globals.hpp"

class Trig {
    public:
        static float idx(uint16_t n);
        static float sin(uint16_t theta);
        static float cos(uint16_t theta);
};