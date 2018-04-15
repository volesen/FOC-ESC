#include "Transform.hpp"
#include "Trig.hpp"

Iab Transform::clarke(const pwm_phases &phases) 
{
	Iab ab;

	// I_alpha = I_a
	ab.a = phases.A;

	// I_beta = 1/sqrt(3) * I_a + 2/sqrt(3) * I_b
	// Iab.Ib = (1/sqrt(3)) * Iabc.Ia + (2/sqrt(3))*Iabc.Ib;
	ab.b = ((float) 0.57735026919 * phases.A + (float) 1.15470053838 * phases.B);

	return ab;
}

pwm_phases Transform::inv_clarke(const Iab &Iab) 
{
	pwm_phases phases;

	phases.A = Iab.a;

	// Iabc.Ib =  -0.5 * Iab.Ia + (sqrt(3)/2) * Iab.Ib;

	phases.B = ((float) -0.5 * Iab.a + (float) 0.8660254039 * Iab.b);
	
	//Iabc.Ic =  -0.5 * Iab.Ia - (sqrt(3)/2) * Iab.Ib;
	phases.C = ((float) -0.5 * Iab.a - (float) 0.8660254039 * Iab.b);

	return phases;
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

Idq Transform::de_phase(const uint16_t &theta, const pwm_phases &phases)
{
	return park(theta, clarke(phases));
}
pwm_phases Transform::to_phase(const uint16_t &theta, const Idq &Idq)
{
	return inv_clarke(inv_park(theta, Idq));
}