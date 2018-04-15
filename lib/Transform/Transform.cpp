#include "Transform.hpp"
#include "Trig.hpp"

Iab Transform::clarke(const Iabc &Iabc) 
{
	Iab ab;

	// I_alpha = I_a
	ab.a = Iabc.a;

	// I_beta = 1/sqrt(3) * I_a + 2/sqrt(3) * I_b
	// Iab.Ib = (1/sqrt(3)) * Iabc.Ia + (2/sqrt(3))*Iabc.Ib;
	ab.b = ((float) 0.57735026919 * Iabc.a + (float) 1.15470053838 * Iabc.b);

	return ab;
}

Iabc Transform::inv_clarke(const Iab &Iab) 
{
	Iabc abc;

	abc.a = Iab.a;

	// Iabc.Ib =  -0.5 * Iab.Ia + (sqrt(3)/2) * Iab.Ib;

	abc.b = ((float) -0.5 * Iab.a + (float) 0.8660254039 * Iab.b);
	
	//Iabc.Ic =  -0.5 * Iab.Ia - (sqrt(3)/2) * Iab.Ib;
	abc.c = ((float) -0.5 * Iab.a - (float) 0.8660254039 * Iab.b);

	return abc;
}

Idq Transform::park(const uint16_t &theta, const Iab &Iab) 
{
	float co = Trig::cos(theta);
	float si = Trig::sin(theta);

	Idq dq;

	dq.d = co*Iab.a + si*Iab.b;
	dq.q = -si*Iab.a + co*Iab.b;

	return dq;
}

Iab Transform::inv_park(const uint16_t &theta, const Idq &Idq) 
{
	float co = Trig::cos(theta);
	float si = Trig::sin(theta);

	Iab ab;

	ab.a = co*Idq.d - si*Idq.q;
	ab.b = si*Idq.d + co*Idq.q;

	return ab;
}

Idq Transform::de_phase(const uint16_t &theta, const Iabc &Iabc)
{
	return park(theta, clarke(Iabc));
}
Iabc Transform::to_phase(const uint16_t &theta, const Idq &Idq)
{
	return inv_clarke(inv_park(theta, Idq));
}