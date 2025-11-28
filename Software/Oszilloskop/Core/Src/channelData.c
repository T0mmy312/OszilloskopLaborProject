/*
 * channelData.c
 *
 *  Created on: Oct 3, 2025
 *      Author: Tommy
 */
#include "channelData.h"

float calculateScale(float maxPM) {
	return maxPM / 1.65; // the calculated voltage from the adc value on the computer is +-1.65V so we have to scale the range to that
}

uint8_t running = 0;
uint16_t dmaBuffer[DMA_BUFFER_LEN];
