#include <stdio.h>
#include <stdlib.h>

typedef struct horse
{
    int ID;
    int posi;
    int posj;
};

typedef struct Player
{
    char name[15];
    struct horse CJ[4];
    int HorseCnt;
    int IsDone;
};

typedef struct Game
{
    int* Order;
    struct Player NP[4];
};
