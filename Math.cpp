#pragma once

/*
	Implementation of lookuptables for sine and cosine without comparisons
*/

#include "Math.hpp"

/*
	Index of mechanical angle in lookup table
*/

float32_t Math::idx(uint16_t theta) {
	int n = theta % ELECTRIC_ROTATION_STEPS;
	return n;
}

/*
	Get n'th element in lookup table for sine
*/

float32_t Math::sin(uint16_t theta) {
	int n = Math::idx(theta);
	return sin_table[n];
}

/*
	Get n'th element in lookup table for cosine
*/

float32_t Math::cos(uint16_t theta) {
	int n = Math::idx(theta);
	return cos_table[n];
}