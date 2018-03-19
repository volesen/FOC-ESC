#pragma once

typedef float float32_t;

typedef int uint16_t;

typedef struct {
	float32_t Ia, Ib, Ic;
} Iabc;

typedef struct {
	float32_t Ia, Ib;
} Iab;

typedef struct {
	float32_t Id, Iq;
} Idq;

class Transform{
	public:
		static void clarke(Iabc &Iabc, Iab &Iab);
		static void inv_clarke(Iab &Iab, Iabc &Iabc);
		static void park(uint16_t &theta, Iab &Iab, Idq &Idq);
		static void inv_park(uint16_t &theta, Idq &Idq, Iab &Iab);
};