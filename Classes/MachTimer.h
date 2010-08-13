//
//  MachTimer.h
//

#ifndef __MACHTIMER_H__
#define __MACHTIMER_H__

#include <assert.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

class MachTimer {
	uint64_t t0;
public:
	MachTimer();
	void start();
	uint64_t elapsed();
	float elapsedSec();
};

#endif