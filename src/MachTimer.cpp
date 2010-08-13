
//
//  MachTimer.m
//

#include "MachTimer.h"
#include "config.h"

#ifdef TARGET_APPLE
static mach_timebase_info_data_t timebase;
#endif

MachTimer::MachTimer() {
#ifdef TARGET_APPLE
	(void) mach_timebase_info(&timebase);
	t0 = mach_absolute_time();
#endif
#ifdef TARGET_ANDROID
	
#endif
}

void MachTimer::start() {
#ifdef __APPLE_CC__
	t0 = mach_absolute_time();
#endif
#ifdef TARGET_ANDROID
	struct timezone tz;
	gettimeofday(&t0,&tz);
#endif

}

#ifdef TARGET_APPLE
// apple's mach_absolute_time is using its own time units and this value is unusable in a platform-independent sense
// actually, this function is obsolete; none of urMus code is calling this function
uint64_t MachTimer::elapsed() {
	return mach_absolute_time() - t0;
}
#endif

float MachTimer::elapsedSec() {
#ifdef TARGET_APPLE
	return ((float)(mach_absolute_time() - t0)) * ((float)timebase.numer) / ((float)timebase.denom) / 1000000000.0f;
#endif
#ifdef TARGET_ANDROID
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv,&tz);
	return (float)(tv.tv_sec-t0.tv_sec)+0.001f*(tv.tv_usec-t0.tv_usec);
#endif
}

