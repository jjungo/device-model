
#ifndef CPLAYGROUND_ICM4000_H
#define CPLAYGROUND_ICM4000_H

#include "accel.h"

struct icm_configuration {
	int fsr;
	int rate_hz;
	enum accel_event_type events;
};

#endif//CPLAYGROUND_ICM4000_H
