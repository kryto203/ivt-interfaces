#include "../inc/sender.h"

const int       PACKET_COUNT        = 1000;
const int       SENSOR_MASK         = 0b111111111111;
const int       SENSOR_DATA_BITS    = 12;
const int       SENSOR_COUNT        = 3;
const int       FRAME_SIZE          = 8;
const uint8_t   START_FRAME         = 0b11111111;
const uint8_t   END_FRAME           = 0b11000111;
const uint8_t   SERVICE_FRAME_MASK  = 0b11000000;
const double    ERR_PROB            = 0.01;

void send_frame(uint8_t *frame, uint16_t *data, int *state, bool man_err)
{
    int data_bits[3] = {0};
    int xor_bits[3] = {0};
    PRINTF_VERBOSE("────────────────────────────────────────────────\n");
    SLEEP_VERBOSE;
    switch (*state)
    {
        case 0:
            *frame = START_FRAME;
            generate_sensor_data(data);
            PRINTF_VERBOSE("Started sending:\nSensor 1: %d\nSensor 2: %d\nSensor 3: %d\n",
                            data[0], data[1], data[2]);
            ++(*state);
            break;
        case 1 ... 12:
            for (int i = 0; i < SENSOR_COUNT; ++i)
                data_bits[i] = (data[i] >> (*state - 1) & 1);
            for (int i = 0; i < SENSOR_COUNT; ++i)
                xor_bits[i] = data_bits[i] ^ data_bits[(i + 1) % SENSOR_COUNT];
            *frame =    xor_bits[2] |
                        xor_bits[1] << 1 |
                        xor_bits[0] << 2 |
                        data_bits[2] << 3 |
                        data_bits[1] << 4 |
                        data_bits[0] << 5;
            ++(*state);
            break;
        case 13:
            *frame = END_FRAME;
            PRINTF_VERBOSE("Sent frame\t\t[%2d/14]: %08b\n", *state + 1, *frame);
            distort_frame(frame, man_err);
            *state = 0;
            return;
        default:
            *frame = START_FRAME;
            *state = 0;
            return;
    }
    PRINTF_VERBOSE("Sent frame\t\t[%2d/14]: %08b\n", *state, *frame);
    distort_frame(frame, man_err);
}

void generate_sensor_data(uint16_t *data)
{
    for (int i = 0; i < SENSOR_COUNT; ++i)
        data[i] = rand() & SENSOR_MASK;
}

void distort_frame(uint8_t *frame, bool manual)
{
    if (manual)
    {
        int change_idx1, change_idx2;
        printf("Enter bit indices to swap: ");
        scanf("%d%d", &change_idx1, &change_idx2);
        if (change_idx1 >= 0)
            *frame ^= 1 << change_idx1;
        if (change_idx2 >= 0)
            *frame ^= 1 << change_idx2;
        return;
    }
    double err1_rand = (double) rand() / RAND_MAX;
    double err2_rand = (double) rand() / RAND_MAX;
    int pos1_rand = rand() % FRAME_SIZE;
    int pos2_rand = rand() % FRAME_SIZE;

    if (err1_rand < ERR_PROB)
    {
        pos1_rand = rand() % FRAME_SIZE;
        (*frame) ^= 1 << pos1_rand;
        if (err2_rand < ERR_PROB)
        {
            while (pos2_rand == pos1_rand)
                pos2_rand = rand() % FRAME_SIZE;
            (*frame) ^= 1 << pos2_rand;
        }
    }
}
