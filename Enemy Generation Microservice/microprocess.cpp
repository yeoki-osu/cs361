#include "enemysharelib.h"

#include <iostream>
#include <string>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Linux only
#include <unistd.h>

using namespace std;

#define EASY 1;
#define MEDIUM 2;
#define HARD 3;


char enemy_names[20][25] = {"Skeleton", 
                            "Zombie", 
                            "Bat", 
                            "Werewolf", 
                            "Serpent", 
                            "Dire Wolf",
                            "Giant Spider",
                            "Banshee",
                            "Giant Snail",
                            "Goblin",
                            "Ork",
                            "Minotaur",
                            "Red Dragon",
                            "Skeleton",
                            "Zombie",
                            "Werewolf",
                            "Bat",
                            "Skeleton",
                            "Zombie",
                            "Bat"};

void _print_enemy(struct enemy* toprint){
    cout << "Name: " << toprint->name;
    cout << "\nHealth: " << toprint->health;
    cout << "\nAttack: " << toprint->attack;
    cout << "\nRarity: " << toprint->rarity << "\n";
}


int main(int argc, char const *argv[])
{
    srand(time(NULL));

    // Set up shared memory.
    key_t key_send_floor = ftok("accesskey.txt", 45);
    key_t key_send_difficulty = ftok("accesskey.txt", 50);
    key_t key_switch = ftok("accesskey.txt", 55);
    key_t key_getenemy = ftok("accesskey.txt", 65);

    size_t key_floorsize = sizeof(int);
    size_t key_diffsize = sizeof(int);
    size_t key_switchsize = (sizeof(bool)); 
    size_t key_enemysize = (sizeof(enemy));

    int floor_id = shmget(key_send_floor, key_floorsize, 0666 | IPC_CREAT);
    int diff_id = shmget(key_send_difficulty,key_diffsize, 0666 | IPC_CREAT);
    int switch_id = shmget(key_switch, key_switchsize, 0666 | IPC_CREAT);
    int enemy_id = shmget(key_getenemy, key_enemysize, 0666 | IPC_CREAT);

    // The actual pipes used
    int* floor_pipe = (int*)shmat(floor_id, NULL, 0);
    int* diff_pipe = (int*)shmat(diff_id, NULL, 0);
    int* switch_pipe = (int*)shmat(switch_id, NULL, 0);   
    struct enemy* enemy_pipe = (enemy*)shmat(enemy_id, NULL, 0);

    // Turn the switch_pipe off right away so that the program waits
    *switch_pipe = false;

    // Loop forever until killed
    while(1){
        while(*switch_pipe == false){
            sleep(DELAY_LOOP);
        }

        int floor = *floor_pipe;
        int difficulty = *diff_pipe;

        // Initializations
        int hp_max = 999;
        int hp_min = -1;

        int atk_max = 999;
        int atk_min = -1;

        switch (difficulty) {
            case 1:
                hp_min = 10 + floor;
                hp_max = 30 + floor - hp_min;

                atk_min = 1 + (int)(floor / 5);
                atk_max = 5 + (int)(floor / 5) - atk_min;
                break;
            case 2:
                hp_min = 10 + (floor * 2);
                hp_max = 50 + (floor * 2) - hp_min;

                atk_min = 1 + ((int)(floor / 5) * 2);
                atk_max = 5 + ((int)(floor / 5) * 2) - atk_min;
                break;
            case 3:
                hp_min = 10 + (floor * 3);
                hp_max = 100 + (floor * 3) - hp_min;

                atk_min = 1 + ((int)(floor / 5) * 3);
                atk_max = 5 + ((int)(floor / 5) * 3) - atk_min;
                break;
            default:
                cout << "error: Got " << difficulty << "\nExiting...\n";
                shmdt(floor_pipe);
                shmdt(diff_pipe);
                shmdt(switch_pipe);
                shmdt(enemy_pipe);
                exit(-1);
                break;
        }

        enemy_pipe->health = (rand() % hp_max) + hp_min + 1;
        enemy_pipe->attack = (rand() % atk_max) + atk_min + 1;

        // Rarity is calculated by doing the health - the minimum divided by the max possible - minimum (already done earlier)
        float temp_rarity_hp = (float)((float)enemy_pipe->health - hp_min)/(float)(hp_max);

        // Debug printing to adjust the ranges and the debug the rarities
        // cout << "Health:" << enemy_pipe->health << "\n";
        // cout << "Max,Min: " << hp_max << ", " << hp_min << "\n";
        // cout << "Calculated Rarity: " << temp_rarity_hp << "\n";
        
        float temp_rarity_atk = (float)((float)enemy_pipe->attack - atk_min)/(float)(atk_max);
        
        // Debug printing to adjust the ranges and the debug the rarities
        // cout << "\nAttack:" << enemy_pipe->attack << "\n";
        // cout << "Max,Min: " << atk_max << ", " << atk_min << "\n";
        // cout << "Calculated Rarity: " << temp_rarity_atk << "\n";

        // The overall rarity is the hp rarity and the attack rarity averaged
        float temp_rarity = ((temp_rarity_hp + temp_rarity_atk)/2)*100;
        enemy_pipe->rarity = temp_rarity;
        // enemy_pipe->name = "Generic Enemy";  
        strcpy(enemy_pipe->name, enemy_names[rand() % 20]);

        if(enemy_pipe->rarity > 100){
            cout << "\n\nERROR, RARITY = " << enemy_pipe->rarity << "\n";
            shmdt(floor_pipe);
            shmdt(diff_pipe);
            shmdt(switch_pipe);
            shmdt(enemy_pipe);
            exit(-1);
        }
        // _print_enemy(enemy_pipe);
        // Reset everything
        *floor_pipe = -1;
        *diff_pipe = -1;
        *switch_pipe = false;
    }

    return 0;
}