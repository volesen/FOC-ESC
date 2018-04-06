#include "Transform.hpp"
#include "Trig.hpp"

Iab Transform::clarke(const Iabc &Iabc) 
{
	Iab ab;

	// I_alpha = I_a
	ab.Ia = Iabc.Ia;

	// I_beta = 1/sqrt(3) * I_a + 2/sqrt(3) * I_b
	// Iab.Ib = (1/sqrt(3)) * Iabc.Ia + (2/sqrt(3))*Iabc.Ib;
	ab.Ib = ((float) 0.57735026919 * Iabc.Ia + (float) 1.15470053838 * Iabc.Ib);

	return ab;
}

Iabc Transform::inv_clarke(const Iab &Iab) 
{
	Iabc abc;

	abc.Ia = Iab.Ia;

	// Iabc.Ib =  -0.5 * Iab.Ia + (sqrt(3)/2) * Iab.Ib;

	abc.Ib = ((float) -0.5 * Iab.Ia + (float) 0.8660254039 * Iab.Ib);
	
	//Iabc.Ic =  -0.5 * Iab.Ia - (sqrt(3)/2) * Iab.Ib;
	abc.Ic = ((float) -0.5 * Iab.Ia - (float) 0.8660254039 * Iab.Ib);

	return abc;
}

Idq Transform::park(const uint16_t &theta, const Iab &Iab) 
{
	float co = Trig::cos(theta);
	float si = Trig::sin(theta);

	Idq dq;

	dq.Id = co*Iab.Ia + si*Iab.Ib;
	dq.Iq = -si*Iab.Ia + co*Iab.Ib;

	return dq;
}

Iab Transform::inv_park(const uint16_t &theta, const Idq &Idq) 
{
	float co = Trig::cos(theta);
	float si = Trig::sin(theta);

	Iab ab;

	ab.Ia = co*Idq.Id - si*Idq.Iq;
	ab.Ib = si*Idq.Id + co*Idq.Iq;

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