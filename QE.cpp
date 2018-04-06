#include "QE.h"

/*
	Initialisation of channel A and B, and determination of initial state
*/

QE::QE(uint8_t channel_A, uint8_t channel_B) {

	// Initialize GPIO pins for channel A and B
	pinMode(channel_A, INPUT_PULLDOWN);
	pinMode(channel_B, INPUT_PULLDOWN);

	// Initialize interupts for Channel A and B
	attachInterrupt(digitalPinToInterrupt(interruptPin), channel_A_rising_edge, RISING);
	attachInterrupt(digitalPinToInterrupt(interruptPin), channel_A_falling_edge, FALLING);

	attachInterrupt(digitalPinToInterrupt(interruptPin), channel_B_rising_edge, RISING);
	attachInterrupt(digitalPinToInterrupt(interruptPin), channel_B_falling_edge, FALLING);

	// Read Channel A and Channel B and determine initial state
	// To be implemented
}

QE::~QE() {}

/*
	State transistions (triggered by interupts)
*/

void QE::channel_A_rising_edge(void) {
	switch(state) {
		case A:
			// State A -> State B
			state = B;
			pos++;
			break;
		case D:
			// State D -> State C
			state = C;
			pos--;
			break;
		default:
			// State transistion from State B and C is illegal, throw exception
			missed_steps++;
			break;
	}
}

void QE::channel_A_falling_edge(void) {
	switch(state) {
		case B:
			// State B -> State A
			state = A;
			pos--;
			break;
		case C:
			// State C -> State D
			state = D;
			pos++;
			break;
		default:
			// State transistion from State A and D is illegal, throw exception
			missed_steps++;
			break;
	}
}

void QE::channel_B_rising_edge(void) {
	switch(state) {
		case A:
			// State A -> State D
			state = D;
			pos--;
			break;
		case B:
			// State B -> State C
			state = C;
			pos++;
			break;
		default:
			// State transistion from State A and C is illegal, throw exception
			missed_steps++;
			break;
	}
}

void QE::channel_B_falling_edge(void) {
	switch(state) {
		case C:
			// State C -> State B
			state = B;
			pos--;
			break;
		case D:
			// State D -> State A
			state = A;
			pos++;
			break;
		default:
			// State transistion to State A and B is illegal, throw exception
			missed_steps++;
			break;
	}
}

int32_t QE::get_pos() {
	return pos
}