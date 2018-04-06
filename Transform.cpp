#pragma once
#include "Transform.hpp"
#include "Math.cpp"

void Transform::clarke(Iabc &Iabc, Iab &Iab) {
	// I_alpha = I_a
	Iab.Ia = Iabc.Ia;

	// I_beta = 1/sqrt(3) * I_a + 2/sqrt(3) * I_b
	// Iab.Ib = (1/sqrt(3)) * Iabc.Ia + (2/sqrt(3))*Iabc.Ib;
	Iab.Ib = ((float32_t) 0.57735026919 * Iabc.Ia + (float32_t) 1.15470053838 * Iabc.Ib);

}

void Transform::inv_clarke(Iab &Iab, Iabc &Iabc) {
	Iabc.Ia = Iab.Ia;

	// Iabc.Ib =  -0.5 * Iab.Ia + (sqrt(3)/2) * Iab.Ib;

	Iabc.Ib = ((float32_t) -0.5 * Iab.Ia + (float32_t) 0.8660254039 * Iab.Ib);
	
	//Iabc.Ic =  -0.5 * Iab.Ia - (sqrt(3)/2) * Iab.Ib;
	Iabc.Ic = ((float32_t) -0.5 * Iab.Ia - (float32_t) 0.8660254039 * Iab.Ib);
}

void Transform::park(uint16_t &theta, Iab &Iab, Idq &Idq) {
	float32_t co = Math::cos(theta);
	float32_t si = Math::sin(theta);

	Idq.Id = co*Iab.Ia + si*Iab.Ib;
	Idq.Iq = -si*Iab.Ia + co*Iab.Ib;
}

void Transform::inv_park(uint16_t &theta, Idq &Idq, Iab &Iab) {
	float32_t co = Math::cos(theta);
	float32_t si = Math::sin(theta);
	
	Iab.Ia = co*Idq.Id - si*Idq.Iq;
	Iab.Ib = si*Idq.Id + co*Idq.Iq;
}