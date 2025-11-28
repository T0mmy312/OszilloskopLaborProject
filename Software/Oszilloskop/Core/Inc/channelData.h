/*
 * channelData.h
 *
 *  Created on: Oct 3, 2025
 *      Author: Tommy
 */

#ifndef INC_CHANNELDATA_H_
#define INC_CHANNELDATA_H_

#include "main.h"

#define DMA_BUFFER_LEN 20000 // 40 kB

float calculateScale(float maxPM); // max plus minus input values

extern uint8_t running;
extern uint16_t dmaBuffer[DMA_BUFFER_LEN];

/*
 * when the start signal is given following data has to be sent before the data packets start:
 * 1 float per channel for the multiplication from -3.3/2 to 3.3/2 to Vmin to Vmax
 * 1 float per channel for freqency of measurement
 * (the current amount of channels is 1)
 *
 * and in each data packet the lenght of the buffer sent in amount of uint16_ts and the buffer is sent
 */

#endif /* INC_CHANNELDATA_H_ */
