#include "../inc/receiver.h"

static const int START_FRAME_RC = 0;
static const int DATA_FRAME_RC = 1;
static const int END_FRAME_RC = 2;
static const int ERR_FRAME_RC = 3;

int receive_frame(uint8_t frame, uint16_t *data, int *state)
{
    int frame_type = analyze_frame(frame);

    if (frame_type == START_FRAME_RC)
        *state = 0;
    switch (*state)
    {
        case 0:
            if (frame_type != START_FRAME_RC)
            {
                PRINTF_VERBOSE("Received incorrect start frame:\t %08b\n", frame);
                return EXIT_FAILURE;
            }
            for (int i = 0; i < SENSOR_COUNT; ++i)
                data[i] = 0;
            ++(*state);
            break;
        case 1 ... 12:
            if (frame_type != DATA_FRAME_RC)
            {
                PRINTF_VERBOSE("Received incorrect data frame:\t %08b\n", frame);
                *state = 0;
                return EXIT_FAILURE;
            }

            for (int i = 0; i < SENSOR_COUNT; ++i)
                data[i] |= (frame >> (5 - i) & 1) << (*state - 1);
            ++(*state);
            break;
        case 13:
            if (frame_type != END_FRAME_RC)
            {
                PRINTF_VERBOSE("Received incorrect end frame:\t %08b\n", frame);
                *state = 0;
                return EXIT_FAILURE;
            }
            PRINTF_VERBOSE("Received frame\t\t[%2d/14]: %08b\n", *state + 1, frame);
            PRINTF_VERBOSE("Finished receiving:\nSensor 1: %d\nSensor 2: %d\nSensor 3: %d\n",
                            data[0], data[1], data[2]);
            *state = 0;
            return EXIT_SUCCESS;
        default:
            *state = 0;
            break;
    }
    PRINTF_VERBOSE("Received frame\t\t[%2d/14]: %08b\n", *state, frame);
    return EXIT_FAILURE;
}

int analyze_frame(uint8_t frame)
{
    if (frame == START_FRAME)
        return START_FRAME_RC;
    if (frame == END_FRAME)
        return END_FRAME_RC;

    if ((frame & SERVICE_FRAME_MASK) != 0)
        return ERR_FRAME_RC;

    int data_bits[3] = {0};
    int xor_bits[3] = {0};

    for (int i = 0; i < SENSOR_COUNT; ++i)
    {
        data_bits[i] = (frame >> (5 - i)) & 1;
        xor_bits[i] = (frame >> (2 - i)) & 1;
    }

    if ((data_bits[0] ^ data_bits[1]) != xor_bits[0] ||
        (data_bits[1] ^ data_bits[2]) != xor_bits[1] ||
        (data_bits[2] ^ data_bits[0]) != xor_bits[2])
        return ERR_FRAME_RC;

    return DATA_FRAME_RC;
}
