/*---------------------------------------------------------------------------------------------
=   Name: Kimberly Yeo
=   Email: yeoki@oregonstate.edu
=   Date: 3/20/2024
=   Course: CS 361-400
=
=   Execution:
=   > ./compile.sh
=   > enemymicroprocess &
=   > g++ main.cpp enemysharelib.o -o run -lncurses
=   > ./run
=   
=   Microservice Termination:
=   > fg 1
=   > CTRL+C
---------------------------------------------------------------------------------------------*/
#include <ncurses.h>            // Ncurses dependency
#include <fstream>              // Leaderboard data storage
#include <string>               // String datatype dependency
#include <time.h>               // srand() / rand()
#include "enemysharelib.h"      // Microservice integration

// Initial leaderboard scores
struct scores
{
    string first = "0";
    string second = "0";
    string third = "0";
};

// Initial weapon datatype
struct weapon
{
    string name = "";
    int attack = 0;
};

// Initial user datatype variables
struct player
{
    int health = 100;
    int potionS = 0;
    int potionM = 0;
    int potionB = 0;
    int floor = 1;
    int difficulty = 0;
    bool alive = true;
    weapon weapons[3];
};

// Initial generated loot drop datatype
struct loot
{
    int drops = 0;
    bool weaponDrop = false;
    weapon weaponAdd;
    int potionS = 0;
    int potionM = 0;
    int potionB = 0;
};

// All weapon types
weapon weaponList[17] =
{
    {"TAUNT", 0},
    {"KICK", 5},
    {"BROKEN BLADE", 10},
    {"STICK", 1},
    {"ROCK", 3},
    {"BOOMERANG", 7},
    {"WINE BOTTLE", 9},
    {"CLUB", 10},
    {"SHURIKEN", 13},
    {"BOW", 15},
    {"DAGGER", 17},
    {"LANCE", 20},
    {"SWORD", 25},
    {"AXE", 28},
    {"CROSSBOW", 30},
    {"GUN", 50},
    {"ROCKET LAUNCHER", 69}
};

// Default starting weapons
weapon defaultWeapons[3] =
{
    weaponList[0],
    weaponList[1],
    weaponList[2]
};

// Possible weapon drops rarity 0-24
weapon weapons25[10] =
{
    weaponList[3],
    weaponList[3],
    weaponList[4],
    weaponList[4],
    weaponList[5],
    weaponList[5],
    weaponList[6],
    weaponList[6],
    weaponList[7],
    weaponList[8]
};

// Possible weapon drops rarity 25-74
weapon weapons75[10] =
{
    weaponList[7],
    weaponList[7],
    weaponList[8],
    weaponList[8],
    weaponList[9],
    weaponList[9],
    weaponList[10],
    weaponList[10],
    weaponList[11],
    weaponList[12]
};

// Possible weapon drops rarity 75-100
weapon weapons100[10] =
{
    weaponList[11],
    weaponList[11],
    weaponList[12],
    weaponList[12],
    weaponList[13],
    weaponList[13],
    weaponList[14],
    weaponList[14],
    weaponList[15],
    weaponList[16],
};

// Global Variables
int totalEnemyHealth = 0;
struct enemy* currEnemy;
struct player currPlayer;
struct loot currLoot;
struct scores currScores;

// Main menu function prototypes
void printMenu(int selectedOption);
void play();
void printPlay();
void printBlankSave();
void printLoadingBar(int progress);
void printDescription();
void checkFile();
void leaderboard();
void printLeaderboard();
void controls();
void printControls();
bool quit(int state);
void printQuit();

// Game function prototypes
void printExit();
void dead();
void printDead();
void playerReset();
void updateLeaderboard();
void floorChoice();
void floorChoiceRun(int selectedOption);
void printChoiceMenu(int selectedOption);
void floor();
void printFloor(int selectedOption);
void printName();
void printHealthBar();
int attack();
void printAttack(int selectedOption);
bool inventory();
void printInventory(int selectedOption);
void info();
void printInfo();
bool floorRun();
void printFloorRun();
void drops();
void dropsWeapon25(int dropChance);
void dropsWeapon75(int dropChance);
void dropsWeapon100(int dropChance);
void checkWeapon();
void dropsPotion25();
void dropsPotion75();
void dropsPotion100();
void printDrops();
void lootReset();

/*---------------------------------------------------------------------------------------------
=   Main menu driver function
---------------------------------------------------------------------------------------------*/
int main()
{
    // Initialize rand() seed
    srand(time(NULL));

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Initialize output colors
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    // Verifies scores.txt or creates it
    checkFile();

    // Main loop
    int ch;                     // Stores user input
    int selectedOption = 0;     // User selected option
    bool running = true;        // Loop flag
    while (running)
    {
        printMenu(selectedOption);

        // Handle user input
        ch = getch();
        switch (ch)
        {
            case KEY_UP:
            case 'w':
            case 'W':
                selectedOption = (selectedOption - 1 + 4) % 4;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                selectedOption = (selectedOption + 1) % 4;
                break;
            case 10:
            case '1':
            case ' ':
                // Handle selected option
                switch (selectedOption)
                {
                    case 0:
                        play();
                        break;
                    case 1:
                        leaderboard();
                        break;
                    case 2:
                        controls();
                        break;
                    case 3:
                        if (quit(0))
                        {
                            running = false;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    endwin();   // Cleanup ncurses
    return 0;
}

/*---------------------------------------------------------------------------------------------
=   Main menu screen display
---------------------------------------------------------------------------------------------*/
void printMenu(int selectedOption)
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "- << CRYPTIC CATACOMBS: SHADOWS OF THE ABYSS >> -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-                   [ PLAY ]                    -");
    mvprintw(4, 0, "-               [ LEADERBOARD ]                 -");
    mvprintw(5, 0, "-                 [ CONTROLS ]                  -");
    mvprintw(6, 0, "-                   [ QUIT ]                    -");
    mvprintw(7, 0, "-                                               -");
    mvprintw(8, 0, "-------------------------------------------------");

    // Highlight the selected option
    attron(A_REVERSE);
    switch (selectedOption)
    {
        case 0:
            mvprintw(3, 21, " PLAY");
            break;
        case 1:
            mvprintw(4, 17, " LEADERBOARD");
            break;
        case 2:
            mvprintw(5, 19, " CONTROLS");
            break;
        case 3:
            mvprintw(6, 21, " QUIT");
            break;
        default:
            break;
    }
    attroff(A_REVERSE);

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Play option driver function
---------------------------------------------------------------------------------------------*/
void play()
{
    printPlay();

    // Initialize cursor position
    int cursorY = 40;
    int cursorX = 29;

    // Set initial selection
    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);

    // Handle user input
    int ch;
    while (true)
    {
        // Move cursor and update selection
        ch = getch();
        switch (ch)
        {
            case KEY_LEFT:
            case 'a':
            case 'A':
                if (cursorX == 45)
                {
                    mvchgat(cursorY, cursorX, 6, A_NORMAL, 0, NULL);
                    cursorX = 29;
                    selectedOption = (selectedOption + 1) % 2;
                    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (cursorX == 29)
                {
                    mvchgat(cursorY, cursorX, 6, A_NORMAL, 0, NULL);
                    cursorX = 45;
                    selectedOption = (selectedOption + 1) % 2;
                    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);
                }
                break;
            case 10:
            case '1':
            case ' ':
                if (selectedOption == 0){
                    playerReset();
                    floorChoice();
                }
                return;
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Play screen display driver function
---------------------------------------------------------------------------------------------*/
void printPlay()
{
    int totalSteps = 5;     // Number of loading bar sections
    int progress = 0;       // Current loading bar progress

    printBlankSave();

    // Loading bar loop
    for (int step = 1; step <= totalSteps; step++)
    {
        usleep(500000); // Sleep for 0.5 seconds (500 milliseconds)

        // Update loading bar
        progress = (step * 100) / totalSteps;
        printLoadingBar(progress);

        refresh();
    }

    printDescription();
}

/*---------------------------------------------------------------------------------------------
=   Play screen display helper function -- base template output
---------------------------------------------------------------------------------------------*/
void printBlankSave()
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------------------------------------");
    mvprintw(1, 0, "-                            << CREATING NEW SAVE >>                          -");
    mvprintw(2, 0, "-                                                                             -");
    mvprintw(3, 0, "-                                                                             -");
    mvprintw(4, 0, "-                                                                             -");
    mvprintw(5, 0, "-                                                                             -");
    mvprintw(6, 0, "-                                                                             -");
    mvprintw(7, 0, "-                                                                             -");
    mvprintw(8, 0, "-                                                                             -");
    mvprintw(9, 0, "-                                                                             -");
    mvprintw(10, 0, "-                                                                             -");
    mvprintw(11, 0, "-                                                                             -");
    mvprintw(12, 0, "-                                                                             -");
    mvprintw(13, 0, "-                                                                             -");
    mvprintw(14, 0, "-                                                                             -");
    mvprintw(15, 0, "-                                                                             -");
    mvprintw(16, 0, "-                                                                             -");
    mvprintw(17, 0, "-                                                                             -");
    mvprintw(18, 0, "-                                                                             -");
    mvprintw(19, 0, "-                                                                             -");
    mvprintw(20, 0, "-                                                                             -");
    mvprintw(21, 0, "-                                                                             -");
    mvprintw(22, 0, "-                                                                             -");
    mvprintw(23, 0, "-                                                                             -");
    mvprintw(24, 0, "-                                                                             -");
    mvprintw(25, 0, "-                                                                             -");
    mvprintw(26, 0, "-                                                                             -");
    mvprintw(27, 0, "-                                                                             -");
    mvprintw(28, 0, "-                                                                             -");
    mvprintw(29, 0, "-                                                                             -");
    mvprintw(30, 0, "-                                                                             -");
    mvprintw(31, 0, "-                                                                             -");
    mvprintw(32, 0, "-                                                                             -");
    mvprintw(33, 0, "-                                                                             -");
    mvprintw(34, 0, "-                                                                             -");
    mvprintw(35, 0, "-                                                                             -");
    mvprintw(36, 0, "-                                                                             -");
    mvprintw(37, 0, "-                                                                             -");
    mvprintw(38, 0, "-                                                                             -");
    mvprintw(39, 0, "-                                                                             -");
    mvprintw(40, 0, "-                                                                             -");
    mvprintw(41, 0, "-                                                                             -");
    mvprintw(42, 0, "-------------------------------------------------------------------------------");

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Play screen display helper function -- progress bar output
---------------------------------------------------------------------------------------------*/
void printLoadingBar(int progress)
{
    mvprintw(3, 28, "Progress: [%d", progress);     // Print current progress

    // Calculate the number of '=' characters to print
    int numBar = (progress * 10) / 100;     // Max 10 '=' for 100%

    // Print the loading bar
    for (int i = 0; i < numBar; i++)
    {
        mvprintw(3, 42 + i, "=");
    }
    for (int i = numBar; i < 10; i++)
    {
        mvprintw(3, 42 + i, " ");
    }
    printw("]");
}

/*---------------------------------------------------------------------------------------------
=   Play screen display helper function -- game description
---------------------------------------------------------------------------------------------*/
void printDescription()
{
    mvprintw(5, 0, "-               Welcome to CRYPTIC CATACOMBS: SHADOWS OF THE ABYSS!           -");
    mvprintw(7, 0, "-     WARNING: If you can't see the options below, make the window bigger     -");
    mvprintw(8, 0, "-   and press 'd', then 'spacebar'. Try again once you're back to the menu.   -");
    mvprintw(10, 0, "-        You are a brave adventurer diving into a dungeon to hone your        -");
    mvprintw(11, 0, "-                   skills toward reaching greater heights.                   -");
    mvprintw(13, 0, "-                Why and to what extent you ask? Don't ask me.                -");
    mvprintw(14, 0, "-                  You are your own driver in this adventure.                 -");
    mvprintw(16, 0, "-   Once you enter the dungeon, you have to keep going until you eventually   -");
    mvprintw(17, 0, "-         perish or choose to quit. There's no end. Just keep killing.        -");
    mvprintw(19, 0, "-            There is no way to resume your progress, so strap in.            -");
    mvprintw(21, 0, "-     Each floor will present you with three difficulties with a range of     -");
    mvprintw(22, 0, "-    possible enemies. The further you go into the dungeon, the harder the    -");
    mvprintw(23, 0, "-  enemies will become. The difficulties you choose will determine the drops. -");
    mvprintw(25, 0, "-    Each enemy will drop at least one HP potion of varying strength, while   -");
    mvprintw(26, 0, "-                  there's a 10 percent chance for a weapon.                  -");
    mvprintw(27, 0, "- Each difficulty has a generation pool of 4 common weapons and 2 rarer ones. -");
    mvprintw(29, 0, "-            Better weapons will automatically replace lesser ones.           -");
    mvprintw(30, 0, "-   You can use potions whenever you can access them. There's no health cap.  -");
    mvprintw(32, 0, "-                         Use these drops to survive.                         -");
    mvprintw(34, 0, "-                    It's dangerous to go alone, take this.                   -");
    mvprintw(36, 0, "-                         +1 [ BROKEN BLADE - 10 DMG ]                        -");
    mvprintw(38, 0, "-                         Good luck, brave adventurer.                        -");
    mvprintw(40, 0, "-                           [ PLAY ]        [ BACK ]                          -");

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Verifies scores.txt
---------------------------------------------------------------------------------------------*/
void checkFile()
{
    ifstream check("scores.txt");
    if (check.peek() == EOF)
    {
        ofstream write("scores.txt");
        write << currScores.first << endl;
        write << currScores.second << endl;
        write << currScores.third;
        write.close();
    }
    check.close();
}

/*---------------------------------------------------------------------------------------------
=   Leaderboard option driver function
---------------------------------------------------------------------------------------------*/
void leaderboard()
{
    printLeaderboard();

    // Initialize cursor position
    int cursorY = 7;
    int cursorX = 22;

    // Set initial selection
    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);

    // Handle user input
    int ch;
    while (true)
    {
        ch = getch();

        switch (ch)
        {
            case 10:
            case '1':
            case ' ':
                return;
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Leaderboard screen display
---------------------------------------------------------------------------------------------*/
void printLeaderboard()
{
    // Get leaderboard data
    ifstream read("scores.txt");
    read >> currScores.first >> currScores.second >> currScores.third;
    read.close();

    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                << LEADERBOARD >>              -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-                 1.  FLOOR                     -");
    mvprintw(4, 0, "-                 2.  FLOOR                     -");
    mvprintw(5, 0, "-                 3.  FLOOR                     -");
    mvprintw(6, 0, "-                                               -");
    mvprintw(7, 0, "-                    [ BACK ]                   -");
    mvprintw(8, 0, "-                                               -");
    mvprintw(9, 0, "-------------------------------------------------");

    // Output floor values
    mvprintw(3, 28, "%s", currScores.first.c_str());
    mvprintw(4, 28, "%s", currScores.second.c_str());
    mvprintw(5, 28, "%s", currScores.third.c_str());

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Controls option driver function
---------------------------------------------------------------------------------------------*/
void controls()
{
    printControls();

    // Initialize cursor position
    int cursorY = 6;
    int cursorX = 22;

    // Set initial selection
    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);

    // Handle user input
    int ch;
    while (true)
    {
        ch = getch();

        switch (ch)
        {
            case 10:
            case '1':
            case ' ':
                return;
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Controls screen display
---------------------------------------------------------------------------------------------*/
void printControls()
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                 << CONTROLS >>                -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-   Cursor  ---  WASD / Arrow Keys              -");
    mvprintw(4, 0, "-   Select  ---  Space Bar / Enter / NUMPAD 1   -");
    mvprintw(5, 0, "-                                               -");
    mvprintw(6, 0, "-                    [ BACK ]                   -");
    mvprintw(7, 0, "-                                               -");
    mvprintw(8, 0, "-------------------------------------------------");

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Quit option driver function
---------------------------------------------------------------------------------------------*/
bool quit(int state)
{
    if (state == 0)
    {
        printQuit();    // Qutting from main menu
    }
    else
    {
        printExit();    // Quitting from floor section
    }

    // Initialize cursor position
    int cursorY = 3;
    int cursorX = 17;

    // Set initial selection
    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 5, A_REVERSE, 0, NULL);

    // Handle user input
    int ch;
    while (true)
    {
        // Move cursor and update selection
        ch = getch();
        switch (ch)
        {
            case KEY_LEFT:
            case 'a':
            case 'A':
                if (cursorX == 28)
                {
                    mvchgat(cursorY, cursorX, 4, A_NORMAL, 0, NULL);
                    cursorX = 17;
                    selectedOption = 0;
                    mvchgat(cursorY, cursorX, 5, A_REVERSE, 0, NULL);
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (cursorX == 17)
                {
                    mvchgat(cursorY, cursorX, 5, A_NORMAL, 0, NULL);
                    cursorX = 28;
                    selectedOption = 1;
                    mvchgat(cursorY, cursorX, 4, A_REVERSE, 0, NULL);
                }
                break;
            case 10:
            case '1':
            case ' ':
                return selectedOption == 0; // Return true for "YES", false for "NO"
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Quit screen display
---------------------------------------------------------------------------------------------*/
void printQuit()
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                << CLOSE GAME >>               -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-               [ YES ]    [ NO ]               -");
    mvprintw(4, 0, "-                                               -");
    mvprintw(5, 0, "-------------------------------------------------");

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Exit screen display
---------------------------------------------------------------------------------------------*/
void printExit()
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                 << END RUN >>                 -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-               [ YES ]    [ NO ]               -");
    mvprintw(4, 0, "-                                               -");
    mvprintw(5, 0, "-------------------------------------------------");

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Death screen driver function
---------------------------------------------------------------------------------------------*/
void dead()
{
    printDead();

    // Initialize cursor position
    int cursorY = 3;
    int cursorX = 22;

    // Set initial selection
    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);

    // Handle user input
    int ch;
    while (true)
    {
        ch = getch();

        switch (ch)
        {
            case 10:
            case '1':
            case ' ':
                return;
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Death screen display
---------------------------------------------------------------------------------------------*/
void printDead()
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                 << YOU DIED >>                -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-                    [ BACK ]                   -");
    mvprintw(4, 0, "-                                               -");
    mvprintw(5, 0, "-------------------------------------------------");

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Resets player object
---------------------------------------------------------------------------------------------*/
void playerReset()
{
    currPlayer.health = 100;
    currPlayer.potionS = 0;
    currPlayer.potionM = 0;
    currPlayer.potionB = 0;
    currPlayer.floor = 1;
    currPlayer.alive = true;

    for (int i = 0; i < 3; i++)
    {
        currPlayer.weapons[i] = defaultWeapons[i];
    }
}

/*---------------------------------------------------------------------------------------------
=   Updates leaderboard scores
---------------------------------------------------------------------------------------------*/
void updateLeaderboard()
{
    // Get leaderboard data
    ifstream read("scores.txt");
    read >> currScores.first >> currScores.second >> currScores.third;
    read.close();

    ofstream write("scores.txt");
    if (currPlayer.floor > stoi(currScores.first))
    {
        write << currPlayer.floor << endl;
        write << currScores.second << endl;
        write << currScores.third;
    }
    else if (currPlayer.floor > stoi(currScores.second))
    {
        write << currScores.first << endl;
        write << currPlayer.floor << endl;
        write << currScores.third;
    }
    else if (currPlayer.floor > stoi(currScores.third))
    {
        write << currScores.first << endl;
        write << currScores.second << endl;
        write << currPlayer.floor;
    }
    write.close();
}

/*---------------------------------------------------------------------------------------------
=   Game main screen 1 driver function -- choosing floor difficulty
---------------------------------------------------------------------------------------------*/
void floorChoice()
{
    int selectedOption = 0;     // User selected option
    int ch;                     // Stores user selected option
    
    while (currPlayer.alive)
    {
        printChoiceMenu(selectedOption);
        
        // Handle user input
        ch = getch();
        switch (ch)
        {
            case KEY_UP:
            case 'w':
            case 'W':
                selectedOption = (selectedOption - 1 + 5) % 5;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                selectedOption = (selectedOption + 1) % 5;
                break;
            case 10:
            case '1':
            case ' ':
                // Handle selected option
                switch (selectedOption)
                {
                    case 0:     // EASY FLOOR
                        floorChoiceRun(selectedOption);
                        break;
                    case 1:     // NORMAL FLOOR
                        floorChoiceRun(selectedOption);
                        break;
                    case 2:     // HARD FLOOR
                        floorChoiceRun(selectedOption);
                        break;
                    case 3:     // CHECK INVENTORY
                        inventory();
                        break;
                    case 4:     // QUIT
                        if (quit(1))
                        {
                            currPlayer.alive = false;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    updateLeaderboard();
}

/*---------------------------------------------------------------------------------------------
=   Game main screen 1 helper function
---------------------------------------------------------------------------------------------*/
void floorChoiceRun(int selectedOption)
{
    currPlayer.difficulty = selectedOption;
    currEnemy = generate_enemy(currPlayer.floor, currPlayer.difficulty + 1);    // Generate enemy
    totalEnemyHealth = currEnemy->health;
    floor();     // Run floor simulation
}

/*---------------------------------------------------------------------------------------------
=   Game main screen 1 screen display
---------------------------------------------------------------------------------------------*/
void printChoiceMenu(int selectedOption)
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-             << CHOOSE YOUR PATH >>            -");
    mvprintw(2, 0, "-                 << FLOOR                      -");
    mvprintw(3, 0, "-                                               -");
    mvprintw(4, 0, "-                 [ EASY FLOOR ]                -");
    mvprintw(5, 0, "-                [ NORMAL FLOOR ]               -");
    mvprintw(6, 0, "-                 [ HARD FLOOR ]                -");
    mvprintw(7, 0, "-               [ CHECK INVENTORY ]             -");
    mvprintw(8, 0, "-                    [ EXIT ]                   -");
    mvprintw(9, 0, "-                                               -");
    mvprintw(10, 0, "-------------------------------------------------");

    // Output floor value
    mvprintw(2, 27, "%d >>", currPlayer.floor);

    // Highlight the selected option
    attron(A_REVERSE);
    switch (selectedOption)
    {
        case 0:
            mvprintw(4, 19, " EASY FLOOR");
            break;
        case 1:
            mvprintw(5, 18, " NORMAL FLOOR");
            break;
        case 2:
            mvprintw(6, 19, " HARD FLOOR");
            break;
        case 3:
            mvprintw(7, 17, " CHECK INVENTORY");
            break;
        case 4:
            mvprintw(8, 22, " EXIT");
            break;
        default:
            break;
    }
    attroff(A_REVERSE);

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Game main screen 2 driver function -- floor functionality
---------------------------------------------------------------------------------------------*/
void floor()
{
    int selectedOption = 0;     // User selected option
    int ch;                     // Stores user selected option
    bool run = false;           // Loop check
    int damage = 0;             // Holder variable

    while (!run)
    {
        printFloor(selectedOption);
        
        // Handle user input
        ch = getch();
        switch (ch)
        {
            case KEY_UP:
            case 'w':
            case 'W':
                selectedOption = (selectedOption - 1 + 4) % 4;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                selectedOption = (selectedOption + 1) % 4;
                break;
            case 10:
            case '1':
            case ' ':
                switch (selectedOption)
                {
                    case 0:     // ATTACK
                        damage = attack();
                        if (damage != -1)
                        {
                            currEnemy->health -= damage;
                            if (currEnemy->health > 0)
                            {
                                currPlayer.health -= currEnemy->attack;
                            }
                        }

                        if (currEnemy->health <= 0)
                        {
                            lootReset();
                            drops();
                            currPlayer.floor++;
                            run = true;   
                        }
                        if (currPlayer.health <= 0)
                        {
                            currPlayer.alive = false;
                            run = true;
                            dead();
                        }

                        break;
                    case 1:     // INVENTORY
                        if (inventory() == true)
                        {
                            currPlayer.health -= currEnemy->attack;
                            if (currPlayer.health <= 0)
                            {
                                currPlayer.alive = false;
                                run = true;
                                dead();
                            }
                        }
                        break;
                    case 2:     // INFORMATION
                        info();
                        break;
                    case 3:     // RUN
                        if (floorRun())
                        {
                            run = true;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Floor screen display
---------------------------------------------------------------------------------------------*/
void printFloor(int selectedOption)
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                 << FLOOR                      -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-                                               -");
    mvprintw(4, 0, "-                                               -");
    mvprintw(5, 0, "-                                               -");
    mvprintw(6, 0, "-               PLAYER HEALTH:                  -");
    mvprintw(7, 0, "-                                               -");
    mvprintw(8, 0, "-                   [ ATTACK ]                  -");
    mvprintw(9, 0, "-                 [ INVENTORY ]                 -");
    mvprintw(10, 0, "-                [ INFORMATION ]                -");
    mvprintw(11, 0, "-                    [ RUN ]                    -");
    mvprintw(12, 0, "-                                               -");
    mvprintw(13, 0, "-------------------------------------------------");

    // Output enemy
    printName();
    printHealthBar();

    // Output floor values
    mvprintw(1, 27, "%d >>", currPlayer.floor);
    mvprintw(6, 31, "%d", currPlayer.health);

    // Highlight the selected option
    attron(A_REVERSE);
    switch (selectedOption)
    {
        case 0:
            mvprintw(8, 21, " ATTACK");
            break;
        case 1:
            mvprintw(9, 19, " INVENTORY");
            break;
        case 2:
            mvprintw(10, 18, " INFORMATION");
            break;
        case 3:
            mvprintw(11, 22, " RUN");
            break;
        default:
            break;
    }
    attroff(A_REVERSE);

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Enemy name screen display
---------------------------------------------------------------------------------------------*/
void printName()
{
    // Set up print location
    mvprintw(3, 1, " ");

    // Check name alignment
    int totalLength = 47;
    int maxNameLength = 12 + 7;
    int buffer = (totalLength - maxNameLength) / 2;
    for (int i = 0; i < buffer; i++)
    {
        printw(" ");
    }

    printw("ENEMY: %s", currEnemy->name);
}

/*---------------------------------------------------------------------------------------------
=   Enemy health screen display
---------------------------------------------------------------------------------------------*/
void printHealthBar()
{
    // Set up print location
    mvprintw(4, 1, " ");

    // Check health bar alignment
    int totalLength = 47;
    int barLength = (totalEnemyHealth / 5) + 13;
    int buffer = (totalLength - barLength) / 2;
    for (int i = 0; i < buffer; i++)
    {
        printw(" ");
    }

    // Print health bar
    printw("HEALTH: [");
    for (int i = 0; i < currEnemy->health / 5; i++)
    {
        printw("=");    // Print segment per 5 HP
    }
    for (int i = currEnemy->health / 5; i < totalEnemyHealth / 5; i++)
    {
        printw(" ");    // Print filling per 5 missing HP
    }
    printw("] %d", currEnemy->health);
}

/*---------------------------------------------------------------------------------------------
=   Attack option driver function
---------------------------------------------------------------------------------------------*/
int attack()
{
    int selectedOption = 0;     // User selected option

    // Handle user input
    int ch;
    while (true)
    {
        printAttack(selectedOption);

        // Move cursor and update selection
        ch = getch();
        switch (ch)
        {
            case KEY_UP:
            case 'w':
            case 'W':
                selectedOption = (selectedOption - 1 + 4) % 4;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                selectedOption = (selectedOption + 1) % 4;
                break;
            case 10:
            case '1':
            case ' ':
                // Return the selected option's value
                if (selectedOption == 0)
                {
                    return currPlayer.weapons[0].attack;
                }
                else if (selectedOption == 1)
                {
                    return currPlayer.weapons[1].attack;
                }
                else if (selectedOption == 2)
                {
                    return currPlayer.weapons[2].attack;
                }
                else
                {
                    return -1;
                }
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Attack screen display
---------------------------------------------------------------------------------------------*/
void printAttack(int selectedOption)
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                 << ATTACKS >>                 -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-                                               -");
    mvprintw(4, 0, "-                                               -");
    mvprintw(5, 0, "-                                               -");
    mvprintw(6, 0, "-                                               -");
    mvprintw(7, 0, "-                   [ BACK ]                    -");
    mvprintw(8, 0, "-                                               -");
    mvprintw(9, 0, "-------------------------------------------------");

    // Output weapon values
    mvprintw(3, 15, "%s", currPlayer.weapons[0].name.c_str());
    mvprintw(4, 15, "%s", currPlayer.weapons[1].name.c_str());
    mvprintw(5, 15, "%s", currPlayer.weapons[2].name.c_str());
    mvprintw(3, 31, "%d DMG", currPlayer.weapons[0].attack);
    mvprintw(4, 31, "%d DMG", currPlayer.weapons[1].attack);
    mvprintw(5, 31, "%d DMG", currPlayer.weapons[2].attack);

    // Highlight the selected option
    attron(A_REVERSE);
    switch (selectedOption)
    {
        case 0:
            mvprintw(3, 12, " >");
            break;
        case 1:
            mvprintw(4, 12, " >");
            break;
        case 2:
            mvprintw(5, 12, " >");
            break;
        case 3:
            mvprintw(7, 21, " BACK");
            break;
        default:
            break;
    }
    attroff(A_REVERSE);

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Inventory driver function
---------------------------------------------------------------------------------------------*/
bool inventory()
{
    int selectedOption = 0;     // User selected option

    // Handle user input
    int ch;
    while (true)
    {
        printInventory(selectedOption);

        // Move cursor and update selection
        ch = getch();
        switch (ch)
        {
            case KEY_UP:
            case 'w':
            case 'W':
                selectedOption = (selectedOption - 1 + 4) % 4;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                selectedOption = (selectedOption + 1) % 4;
                break;
            case 10:
            case '1':
            case ' ':
                // Return the selected option's value
                if (selectedOption == 0)
                {
                    if (currPlayer.potionS > 0)
                    {
                        currPlayer.potionS--;
                        currPlayer.health += 10;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (selectedOption == 1)
                {
                    if (currPlayer.potionM > 0)
                    {
                        currPlayer.potionM--;
                        currPlayer.health += 50;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (selectedOption == 2)
                {
                    if (currPlayer.potionB > 0)
                    {
                        currPlayer.potionB--;
                        currPlayer.health += 100;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return true;
                }
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Inventory screen display
---------------------------------------------------------------------------------------------*/
void printInventory(int selectedOption)
{
    int lowest = 101;   // Holder for lowest DMG attack
    int highest = 0;    // Holder for highest DMG attack

    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                << INVENTORY >>                -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-      PLAYER HEALTH         ---                -");
    mvprintw(4, 0, "-      PLAYER ATTACK RANGE   ---                -");
    mvprintw(5, 0, "-      SMALL POTION (10 HP)  ---                -");
    mvprintw(6, 0, "-      MID POTION (50 HP)    ---                -");
    mvprintw(7, 0, "-      BIG POTION (100 HP)   ---                -");
    mvprintw(8, 0, "-                                               -");
    mvprintw(9, 0, "-                    [ BACK ]                   -");
    mvprintw(10, 0, "-                                               -");
    mvprintw(11, 0, "-------------------------------------------------");

    // Store values for attack range
    for (int i = 0; i < 3; i++)
    {
        if (currPlayer.weapons[i].attack < lowest)
        {
            lowest = currPlayer.weapons[i].attack;
        }
        if (currPlayer.weapons[i].attack > highest)
        {
            highest = currPlayer.weapons[i].attack;
        }
    }

    // Output inventory values
    mvprintw(3, 33, "%d HP", currPlayer.health);
    mvprintw(4, 33, "%d-%d DMG", lowest, highest);
    mvprintw(5, 33, "%d", currPlayer.potionS);
    mvprintw(6, 33, "%d", currPlayer.potionM);
    mvprintw(7, 33, "%d", currPlayer.potionB);

    // Highlight the selected option
    attron(A_REVERSE);
    switch (selectedOption)
    {
        case 0:
            mvprintw(5, 4, " >");
            break;
        case 1:
            mvprintw(6, 4, " >");
            break;
        case 2:
            mvprintw(7, 4, " >");
            break;
        case 3:
            mvprintw(9, 22, " BACK");
            break;
        default:
            break;
    }
    attroff(A_REVERSE);
    
    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Information option driver function
---------------------------------------------------------------------------------------------*/
void info()
{
    printInfo();

    // Initialize cursor position
    int cursorY = 9;
    int cursorX = 22;

    // Set initial selection
    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);

    // Handle user input
    int ch;
    while (true)
    {
        ch = getch();
        switch (ch)
        {
            case 10:
            case '1':
            case ' ':
                return;
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Floor information screen display
---------------------------------------------------------------------------------------------*/
void printInfo()
{
    int lowest = 100;   // Holder for lowest DMG attack
    int highest = 0;    // Holder for highest DMG attack

    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                << FLOOR INFO >>               -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-     PLAYER HEALTH        ---                  -");
    mvprintw(4, 0, "-     PLAYER ATTACK RANGE  ---                  -");
    mvprintw(5, 0, "-     ENEMY HEALTH         ---                  -");
    mvprintw(6, 0, "-     ENEMY ATTACK         ---                  -");
    mvprintw(7, 0, "-     ENEMY RARITY         ---                  -");
    mvprintw(8, 0, "-                                               -");
    mvprintw(9, 0, "-                    [ BACK ]                   -");
    mvprintw(10, 0, "-                                               -");
    mvprintw(11, 0, "-------------------------------------------------");

    // Store values for attack range
    for (int i = 0; i < 3; i++)
    {
        if (currPlayer.weapons[i].attack < lowest)
        {
            lowest = currPlayer.weapons[i].attack;
        }
        if (currPlayer.weapons[i].attack > highest)
        {
            highest = currPlayer.weapons[i].attack;
        }
    }

    // Output floor values
    mvprintw(3, 33, "%d HP", currPlayer.health);
    mvprintw(4, 33, "%d-%d DMG", lowest, highest);
    mvprintw(5, 33, "%d HP", currEnemy->health);
    mvprintw(6, 33, "%d DMG", currEnemy->attack);
    mvprintw(7, 33, "%d", currEnemy->rarity);

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Run option driver function
---------------------------------------------------------------------------------------------*/
bool floorRun()
{
    printFloorRun();

    // Initialize cursor position
    int cursorY = 3;
    int cursorX = 17;

    // Set initial selection
    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 5, A_REVERSE, 0, NULL);

    // Handle user input
    int ch;
    while (true)
    {
        // Move cursor and update selection
        ch = getch();
        switch (ch)
        {
            case KEY_LEFT:
            case 'a':
            case 'A':
                if (cursorX == 28)
                {
                    mvchgat(cursorY, cursorX, 4, A_NORMAL, 0, NULL);
                    cursorX = 17;
                    selectedOption = 0;
                    mvchgat(cursorY, cursorX, 5, A_REVERSE, 0, NULL);
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (cursorX == 17)
                {
                    mvchgat(cursorY, cursorX, 5, A_NORMAL, 0, NULL);
                    cursorX = 28;
                    selectedOption = 1;
                    mvchgat(cursorY, cursorX, 4, A_REVERSE, 0, NULL);
                }
                break;
            case 10:
            case '1':
            case ' ':
                return selectedOption == 0; // Return true for "YES", false for "NO"
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Run screen display
---------------------------------------------------------------------------------------------*/
void printFloorRun()
{
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                   << RUN >>                   -");
    mvprintw(2, 0, "-                                               -");
    mvprintw(3, 0, "-               [ YES ]    [ NO ]               -");
    mvprintw(4, 0, "-                                               -");
    mvprintw(5, 0, "-------------------------------------------------");

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Drops screen driver function
---------------------------------------------------------------------------------------------*/
void drops()
{
    // Weapon generation
    int dropChance = rand() % 10 + 1;   // Generate drop chance of 1/10
    dropsWeapon25(dropChance);
    dropsWeapon75(dropChance);
    dropsWeapon100(dropChance);
    checkWeapon();

    // Potion generation
    dropsPotion25();
    dropsPotion75();
    dropsPotion100();

    // Display drops screen
    printDrops();

    // Initialize cursor position
    int cursorY = currLoot.drops + 4;
    int cursorX = 22;

    // Set initial selection
    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);

    // Handle user input
    int ch;
    while (true)
    {
        ch = getch();
        switch (ch)
        {
            case 10:
            case '1':
            case ' ':
                return;
                break;
            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Drops helper function -- weapon rarity 0-24
---------------------------------------------------------------------------------------------*/
void dropsWeapon25(int dropChance)
{
    if (dropChance == 1)
    {
        if (currEnemy->rarity >= 0 && currEnemy->rarity < 25)
        {
            int weaponType = rand() % 9;        // Weapon 0-9
            currLoot.weaponAdd = weapons25[weaponType];
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Drops helper function -- weapon rarity 25-74
---------------------------------------------------------------------------------------------*/
void dropsWeapon75(int dropChance)
{
    if (dropChance == 1)
    {
        if (currEnemy->rarity >= 25 && currEnemy->rarity < 75)
        {
            int weaponType = rand() % 9;        // Weapon 0-9
            if (currPlayer.difficulty == 0)
            {
                currLoot.weaponAdd = weapons25[weaponType];
            }
            else if (currPlayer.difficulty == 1 || currPlayer.difficulty == 2)
            {
                currLoot.weaponAdd = weapons75[weaponType];
            }
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Drops helper function -- weapon rarity 75-100
---------------------------------------------------------------------------------------------*/
void dropsWeapon100(int dropChance)
{
    if (dropChance == 1)
    {
        if (currEnemy->rarity >= 75 && currEnemy->rarity <= 100)
        {
            int weaponType = rand() % 9;        // Weapon 0-9
            if (currPlayer.difficulty == 0 || currPlayer.difficulty == 1)
            {
                currLoot.weaponAdd = weapons75[weaponType];
            }
            else if (currPlayer.difficulty == 2)
            {
                currLoot.weaponAdd = weapons100[weaponType];
            }
        }
    }
}

/*---------------------------------------------------------------------------------------------
=   Drops helper function -- weapon generation
---------------------------------------------------------------------------------------------*/
void checkWeapon()
{
    int lowest = 101;       // Holder for lowest DMG attack
    int lowestSlot = 0;     // Holder for lowest's slot

    if (currLoot.weaponAdd.attack > 0)
    {
        // Update weapon drop flags
        currLoot.drops++;
        currLoot.weaponDrop = true;

        // Find current lowest DMG weapon
        for (int i = 0; i < 3; i++)
        {
            if (currPlayer.weapons[i].attack < lowest)
            {
                lowest = currPlayer.weapons[i].attack;
                lowestSlot = i;
            }
        }

        // Replace lowest weapon with new drop
        currPlayer.weapons[lowestSlot].name = currLoot.weaponAdd.name;
        currPlayer.weapons[lowestSlot].attack = currLoot.weaponAdd.attack;
    }
}

/*---------------------------------------------------------------------------------------------
=   Drops helper function -- potion rarity 0-24
---------------------------------------------------------------------------------------------*/
void dropsPotion25()
{
    if (currEnemy->rarity >= 0 && currEnemy->rarity < 25)
    {
        currLoot.potionS++;
        currLoot.drops++;
        currPlayer.potionS++;
    }
}

/*---------------------------------------------------------------------------------------------
=   Drops helper function -- potion rarity 25-74
---------------------------------------------------------------------------------------------*/
void dropsPotion75()
{
    if (currEnemy->rarity >= 25 && currEnemy->rarity < 75)
    {
        if (currPlayer.difficulty == 0)
        {
            currLoot.potionS++;
            currPlayer.potionS++;
        }
        else if (currPlayer.difficulty == 1 || currPlayer.difficulty == 2)
        {
            currLoot.potionM++;
            currPlayer.potionM++;
        }
        currLoot.drops++;
    }
}

/*---------------------------------------------------------------------------------------------
=   Drops helper function -- potion rarity 75-100
---------------------------------------------------------------------------------------------*/
void dropsPotion100()
{
    if (currEnemy->rarity >= 75 && currEnemy->rarity <= 100)
    {
        if (currPlayer.difficulty == 0 || currPlayer.difficulty == 1)
        {
            currLoot.potionM++;
            currPlayer.potionM++;
        }
        else if (currPlayer.difficulty == 2)
        {
            currLoot.potionB++;
            currPlayer.potionB++;
        }
        currLoot.drops++;
    }
}

/*---------------------------------------------------------------------------------------------
=   Drops screen display
---------------------------------------------------------------------------------------------*/
void printDrops()
{
    int counter = currLoot.drops;   // Loop flag
    clear();

    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-               << ENEMY DROPS >>               -");
    mvprintw(2, 0, "-                                               -");

    for (int i = 3; i < currLoot.drops + 4; i++)
    {
        if (counter > 0)
        {
            mvprintw(i, 0, "-             +1                                -");
            if (currLoot.weaponDrop == true)
            {
                mvprintw(i, 17, "[ %s - %d DMG ]", currLoot.weaponAdd.name.c_str(), currLoot.weaponAdd.attack);
                currLoot.weaponDrop = false;
                counter--;
            }
            else if (currLoot.potionS > 0)
            {
                mvprintw(i, 17, "[ SMALL POTION ]");
                currLoot.potionS--;
                counter--;
            }
            else if (currLoot.potionM > 0)
            {
                mvprintw(i, 18, "[ MID POTION ]");
                currLoot.potionM--;
                counter--;
            }
            else if (currLoot.potionB > 0)
            {
                mvprintw(i, 18, "[ BIG POTION ]");
                currLoot.potionB--;
                counter--;
            }
        }
        else
        {
            mvprintw(i, 0, "-                                               -");
            mvprintw(i + 1, 0, "-                    [ BACK ]                   -");
            mvprintw(i + 2, 0, "-                                               -");
            mvprintw(i + 3, 0, "-------------------------------------------------");
        }
    }

    refresh();
}

/*---------------------------------------------------------------------------------------------
=   Resets loot object
---------------------------------------------------------------------------------------------*/
void lootReset()
{
    currLoot.drops = 0;
    currLoot.weaponDrop = false;
    currLoot.weaponAdd.name = "";
    currLoot.weaponAdd.attack = 0;
    currLoot.potionS = 0;
    currLoot.potionM = 0;
    currLoot.potionB = 0;
}