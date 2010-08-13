
//
//  MachTimer.m
//

#include "MachTimer.h"

static mach_timebase_info_data_t timebase;

MachTimer::MachTimer() {
	(void) mach_timebase_info(&timebase);
	t0 = mach_absolute_time();
}

void MachTimer::start() {
	t0 = mach_absolute_time();
}

uint64_t MachTimer::elapsed() {
	return mach_absolute_time() - t0;
}

float MachTimer::elapsedSec() {
	return ((float)(mach_absolute_time() - t0)) * ((float)timebase.numer) / ((float)timebase.denom) / 1000000000.0f;
}

