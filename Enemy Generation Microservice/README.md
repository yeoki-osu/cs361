# 361-enemy-microservice
## About
This program is a microservice designed for RPG dungeon crawls. It takes in two parameters and returns a balanced enemy back to the user. It communicates between programs via shared memory. This code only functions on a linux machine.

## Restrictions
- You must be able to use shared memory on whatever computer you are running the code on. 
- You must run the code on a linux machine
- You must compile with the `enemysharelib.o` object
- You ***MUST*** run the `enemymicroservice &` command **BEFORE** you attempt to use the `generate_enemy` function provided. 
    - Not following this instructions will generate undefined behavior and may crash your program.
    - If the `enemymicroservice` process is not running in the background the code will not work


## Installation & Compilation Instructions
1. Clone the github repo
2. You may need to fix the bash scripts permissions. Run the command `chmod 777 compile.sh`
3. Compile by running the bash script. Use the command `./compile.sh`
4. The microservice is now installed and compiled.


## Using the Microservice
1. Ensure you have followed the above steps.
2. Run the microservice process in the background using the command `enemymicroservice &`
    - This is a critical step. Follow these instructions in this order.
3. You can use the provided function "generate_enemy" by including the `enemysharelib.h` library. At the top of your code put the line `#include enemysharelib.h`
4. When you compile your code make sure you include the `enemysharelib.o` object. An example can be found in the `compile.sh` file.
5. You may now run `generate_enemy()`
6. To kill the microservice you have two options
    - **A**
        1. Run the `jobs` command
        2. Type `fg job_id` where job_id is the microservices job id.
        3. The microservice should be running in the foreground now. 
        4. press `Ctrl + C`
    - **B**
        1. Run the `ps` command
        2. Type `kill -9 PID` where PID is the microservices process ID (usually a six digit number).

## Generate Enemy Documentation
1. The prototype for the function is as follows: `struct enemy* generate_enemy(int floor, int difficulty)`
2. The function returns a dynamically allocated enemy with a generated health, attack, and rarity. It will return a name in the future.
3. The first parameter is an integer representing the floor that the user is on. It is expected to be an integer above 0 and below INT_MAX.
4. The second parameter is an integer representing the difficulty the user selected. It is expected to be an integer between 1 and 3. 

## Example Code
1. After compiling using `compile.sh` run the microprocess in the background using `enemymicroprocess &`
2. Run the `runexample` executable
3. Input the desired values
4. Watch an enemy struct be printed


## Sequence Diagram
![A picture of a UML diagram](./Sequence%20diagram.png)