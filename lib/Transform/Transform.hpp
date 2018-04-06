#pragma once

#include <stdint.h>

typedef struct {
	float Ia, Ib, Ic;
} Iabc;

typedef struct {
	float Ia, Ib;
} Iab;

typedef struct {
	float Id, Iq;
} Idq;

class Transform{
	public:
		static void clarke(Iabc &Iabc, Iab &Iab);
		static void inv_clarke(Iab &Iab, Iabc &Iabc);
		static void park(uint16_t &theta, Iab &Iab, Idq &Idq);
		static void inv_park(uint16_t &theta, Idq &Idq, Iab &Iab);
};