#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include "includes.h"

int receive_frame(uint8_t frame, uint16_t *data, int *state);
int analyze_frame(uint8_t frame);

#endif  // __RECEIVER_H__
