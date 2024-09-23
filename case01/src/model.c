#include "../inc/model.h"

static const double RESTART_CHANCE = 0.01;

void model(bool man_err)
{
    int packet_cnt = 0;
    int sender_state = 0;
    int receiver_state = 0;
    uint16_t sensor_data[3] = {0};
    uint16_t received_data[3] = {0};
    uint8_t frame = 0;
    double restart_rand;
    int err_cnt = 0;
    int cur_err;
    srand(time(NULL));
    while (packet_cnt < PACKET_COUNT)
    {
        restart_rand = (double) rand() / RAND_MAX;
        if (restart_rand < RESTART_CHANCE)
        {
            PRINTF_VERBOSE("Sender restarted\n");
            SLEEP_VERBOSE;
            sender_state = 0;
        }
        send_frame(&frame, sensor_data, &sender_state, man_err);
        if (receive_frame(frame, received_data, &receiver_state) == EXIT_SUCCESS)
        {
            cur_err = 0;
            for (int i = 0; i < 3; ++i)
            {
                if (sensor_data[i] != received_data[i])
                    cur_err = 1;
            }
            if (cur_err)
                ++err_cnt;
            ++packet_cnt;
        }
    }
    printf("Sent packets:\t\t%d\n", packet_cnt);
    printf("Correct packets:\t%d\n", packet_cnt - err_cnt);
    printf("incorrect packets:\t%d\n", err_cnt);
}
