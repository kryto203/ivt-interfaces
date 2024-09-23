#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

#ifdef V
#define PRINTF_VERBOSE(...) printf(__VA_ARGS__)
#define SLEEP_VERBOSE sleep(1)
#else
#define PRINTF_VERBOSE(...)
#define SLEEP_VERBOSE
#endif

extern const int        PACKET_COUNT;
extern const int        SENSOR_MASK;
extern const int        SENSOR_DATA_BITS;
extern const int        SENSOR_COUNT;
extern const int        FRAME_SIZE;
extern const uint8_t   START_FRAME;
extern const uint8_t   END_FRAME;
extern const uint8_t   SERVICE_FRAME_MASK;

#endif  // __INCLUDES_H__
