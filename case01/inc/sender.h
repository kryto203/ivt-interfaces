#ifndef __SENDER_H__
#define __SENDER_H__

#include "includes.h"

void send_frame(uint8_t *frame, uint16_t *data, int *state, bool man_err);
void generate_sensor_data(uint16_t *data);
void distort_frame(uint8_t *frame, bool manual);

#endif  // __SENDER_H__
