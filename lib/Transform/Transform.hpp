#pragma once

#include <stdint.h>
#include "ESC_Globals.hpp"

struct Iab{
	float a, b;
};

struct Idq {
	float d, q;
};

class Transform{
	public:
		static Iab clarke(const pwm_phases &phases);
		static pwm_phases inv_clarke(const Iab &Iab);
		static Idq park(const uint16_t &theta, const Iab &Iab);
		static Iab inv_park(const uint16_t &theta, const Idq &Idq);

		static Idq de_phase(const uint16_t &theta, const pwm_phases &phases);
		static pwm_phases to_phase(const uint16_t &theta, const Idq &Idq);
};