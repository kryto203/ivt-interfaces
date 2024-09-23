#include "../inc/includes.h"
#include "../inc/model.h"

int main(int argc, char **argv)
{
    if (argc != 1 && strcmp(argv[1], "-m") == 0)
        model(1);
    else
        model(0);
}
