#pragma once

#define MECHANICAL_ROTATION_STEPS 2048	// 2048 steps per mechanical rotation
#define ELECTRIC_ROTATION_STEPS 342		// ceil(MECHANICAL_ROTATION_STEPS / 6)


#define NUM_MOTORS 2

enum motor_id
{
    motor0,
    motor1
};