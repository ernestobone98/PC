#include <stdio.h>
#include <stdlib.h>

typedef struct horse
{
    int HID;                //Horse ID
    char name[3];
    int posi;
    int posj;
};

typedef struct Player
{
    char name[15];
    struct horse CJ[4];
    int HorseCnt;
};

typedef struct Game
{
    int* Order;
    struct Player NP[4];
};
