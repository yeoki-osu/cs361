#include "enemysharelib.h"

void _print_enemy(struct enemy* toprint){
    cout << "Name: " << toprint->name;
    cout << "\nHealth: " << toprint->health;
    cout << "\nAttack: " << toprint->attack;
    cout << "\nRarity: " << toprint->rarity << "\n";
}

struct enemy* generate_enemy(int floor, int difficulty){
    // It can store one enemy, two numbers (the floor and the difficulty), 
    // and one character (to keep track of if it is sending or waiting)
    key_t key_send_floor = ftok("accesskey.txt", 45);
    key_t key_send_difficulty = ftok("accesskey.txt", 50);
    key_t key_switch = ftok("accesskey.txt", 55);
    key_t key_getenemy = ftok("accesskey.txt", 65);

    // Room for the floor integer
    size_t key_floorsize = sizeof(int);

    // Room for the difficulty integer
    size_t key_diffsize = sizeof(int);

    // Since it's page aligned it doesn't actually matter much how many bytes we allocate
    // here. I'm doing 11 to give some wiggle room.
    size_t key_switchsize = (sizeof(bool)); 

    // The enemy that is returned
    size_t key_enemysize = (sizeof(enemy));

    // Get the id for key_send. Open with all perms or'd with create it
    int floor_id = shmget(key_send_floor, key_floorsize, 0666 | IPC_CREAT);
    int diff_id = shmget(key_send_difficulty,key_diffsize, 0666 | IPC_CREAT);
    // Get the other ids. 
    int switch_id = shmget(key_switch, key_switchsize, 0666 | IPC_CREAT);
    int enemy_id = shmget(key_getenemy, key_enemysize, 0666 | IPC_CREAT);


    // Create the actual memory using the id and letting it pick any memory it wants. 
    // We don't need any flags.
    int* floor_pipe = (int*)shmat(floor_id, NULL, 0);
    
    int* diff_pipe = (int*)shmat(diff_id, NULL, 0);

    int* switch_pipe = (int*)shmat(switch_id, NULL, 0);   

    struct enemy* enemy_pipe = (enemy*)shmat(enemy_id, NULL, 0);

    // Turn the switch pipe off right away
    *switch_pipe = false;

    // 1 easy, 2 medium, 3 hard
    int difficultyNum = difficulty;
    int floorNum = floor;

    *floor_pipe = floorNum;
    *diff_pipe = difficultyNum;

    // Signal that we want a new enemy
    *switch_pipe = true;

    // Wait until the new enemy is created and the switch is toggled off
    // cout << "\nRequested a new enemy\n";
    while(*switch_pipe == true){
        sleep(DELAY_LOOP);
    }
    // cout << "\nGot a new enemy\n";

    // Capture the outputted enemy into a new enemy struct
    struct enemy* output_enemy = new enemy;
    output_enemy->attack = enemy_pipe->attack;
    output_enemy->health = enemy_pipe->health;
    // output_enemy->name = enemy_pipe->name;
    strcpy(output_enemy->name, enemy_pipe->name);
    output_enemy->rarity = enemy_pipe->rarity;

    // cout << "\nEnemy received!\n";
    
    // _print_enemy(output_enemy);

    // let go of everything
    shmdt(floor_pipe);
    shmdt(diff_pipe);
    shmdt(switch_pipe);
    shmdt(enemy_pipe);
    enemy_pipe = NULL;
    return output_enemy;    
}