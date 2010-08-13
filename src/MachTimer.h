//
//  MachTimer.h
//

#ifndef __MACHTIMER_H__
#define __MACHTIMER_H__

#ifdef TARGET_APPLE
#include <assert.h>
#include <unistd.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#ifdef TARGET_ANDROID
#include <sys/time.h>
#endif

#include "config.h"

class MachTimer {
#ifdef TARGET_APPLE
	uint64_t t0;
#endif
#ifdef TARGET_ANDROID
	struct timeval t0;
#endif
	
public:
	MachTimer();
	void start();
#ifdef TARGET_APPLE
	uint64_t elapsed();
#endif
	float elapsedSec();
};

#endif