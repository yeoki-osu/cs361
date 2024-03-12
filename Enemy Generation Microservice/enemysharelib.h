#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>

// Linux only
#include <unistd.h>

using namespace std;

#define DELAY_LOOP 0.25
struct enemy
{
    int health;
    int attack;
    int rarity;
    char name[25];
};

void _print_enemy(struct enemy* toprint);


struct enemy* generate_enemy(int floor, int difficulty);