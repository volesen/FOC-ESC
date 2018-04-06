#pragma once

#include <stdint.h>
#include "Trig.hpp"

struct Iabc {
	float Ia, Ib, Ic;
};

struct Iab{
	float Ia, Ib;
};

struct Idq {
	float Id, Iq;
};

class Transform{
	public:
		static Iab clarke(const Iabc &Iabc);
		static Iabc inv_clarke(const Iab &Iab);
		static Idq park(const uint16_t &theta, const Iab &Iab);
		static Iab inv_park(const uint16_t &theta, const Idq &Idq);

		static Idq de_phase(const uint16_t &theta, const Iabc &Iabc);
		static Iabc to_phase(const uint16_t &theta, const Idq &Idq);
};