#pragma once

/*
	Implementation of lookuptables for sine and cosine without comparisons
*/

#include "Trig.hpp"

/*
	Index of mechanical angle in lookup table
*/

float Trig::idx(uint16_t theta) {
	int n = theta % ELECTRIC_ROTATION_STEPS;
	return n;
}

/*
	Get n'th element in lookup table for sine
*/

float Trig::sin(uint16_t theta) {
	int n = Trig::idx(theta);
	return sin_table[n];
}

/*
	Get n'th element in lookup table for cosine
*/

float Trig::cos(uint16_t theta) {
	int n = Trig::idx(theta);
	return cos_table[n];
}