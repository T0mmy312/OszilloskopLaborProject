#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <config.h>

struct Channel {
	double* array;
	uint16_t* tickStep;
	size_t len;
	size_t maxLen;
};

extern struct Channel channels[NUM_CHANNELS];

void initChannels();

#endif // _CHANNEL_H_
