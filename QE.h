#pragma once

class QE {
	public:
		QE(uint8_t channel_A, uint8_t channel_B); // Constructor - initialisation
		~QE(); // Destructor

		/*
			State transitions
		*/

		void channel_A_rising_edge(void);
		void channel_A_falling_edge(void);

		void channel_B_rising_edge(void);
		void channel_B_falling_edge(void);

		/*
			Getter method for the position counter
		*/

		int32_t get_pos();

	private:
		/*
			State definitions and state variable
		*/

		enum states {A, B, C, D};
		uint8_t state;

		/*
			Position counter and number of illegal state transitions.
			A clockwise step, will increment the position counter
		*/

		int32_t pos = 0;
		uint32_t missed_steps = 0;
};
