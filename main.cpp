// g++ main.cpp -o run -lncurses

#include <ncurses.h>
#include <unistd.h>

// Function to print the menu with highlighted current option
void printMenu(int selectedOption) {
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "- << CRYPTIC CATACOMBS: SHADOWS OF THE ABYSS >> -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print menu options
    mvprintw(3, 0, "-                   [ PLAY ]                    -");
    mvprintw(4, 0, "-               [ LEADERBOARD ]                 -");
    mvprintw(5, 0, "-                 [ CONTROLS ]                  -");
    mvprintw(6, 0, "-                   [ QUIT ]                    -");
    mvprintw(7, 0, "-                                               -");
    mvprintw(8, 0, "-------------------------------------------------");

    // Highlight the selected option
    attron(A_REVERSE);
    switch (selectedOption) {
        case 0:
            mvprintw(3, 19, " [ PLAY ]");
            break;
        case 1:
            mvprintw(4, 15, " [ LEADERBOARD ]");
            break;
        case 2:
            mvprintw(5, 17, " [ CONTROLS ]");
            break;
        case 3:
            mvprintw(6, 19, " [ QUIT ]");
            break;
        default:
            break;
    }
    attroff(A_REVERSE);

    // Refresh the screen
    refresh();
}

// Function to display the play menu and get user selection (New Game or Load Game)
int playMenu() {
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                  << PLAY >>                   -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print menu options
    mvprintw(3, 0, "-                 [ NEW GAME ]                  -");
    mvprintw(4, 0, "-                [ LOAD  SAVE ]                 -");
    mvprintw(5, 0, "-                                               -");
    mvprintw(6, 0, "-------------------------------------------------");

    // Highlight the first option by default
    attron(A_REVERSE);
    mvprintw(3, 17, " [ NEW GAME ]");
    attroff(A_REVERSE);

    // Refresh the screen
    refresh();

    // Handle user input
    int ch;
    int selectedOption = 0;
    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (selectedOption > 0) {
                    mvprintw(selectedOption + 3, 16, " [ LOAD  SAVE ]");
                    selectedOption--;
                    attron(A_REVERSE);
                    mvprintw(selectedOption + 3, 17, " [ NEW GAME ]");
                    attroff(A_REVERSE);
                }
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                if (selectedOption < 1) {
                    mvprintw(selectedOption + 3, 17, " [ NEW GAME ]");
                    selectedOption++;
                    attron(A_REVERSE);
                    mvprintw(selectedOption + 3, 16, " [ LOAD  SAVE ]");
                    attroff(A_REVERSE);
                }
                break;
            case 10: // Enter key
            case '1':
            case ' ':
                return selectedOption; // Return the selected option index
                break;
            default:
                break;
        }
    }
}

int attack(){
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                 << ATTACKS >>                 -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print attack options
    mvprintw(3, 0, "-               KICK   ---   5 DMG              -");
    mvprintw(4, 0, "-               AXE    ---   10 DMG             -");
    mvprintw(5, 0, "-               GUN    ---   20 DMG             -");
    mvprintw(6, 0, "-                                               -");
    mvprintw(7, 0, "-------------------------------------------------");

    // Initialize cursor position
    int cursorY = 3;
    int cursorX = 14;

    // Highlight the '>' character by default
    attron(A_REVERSE);
    mvprintw(cursorY, cursorX, ">");
    attroff(A_REVERSE);

    // Refresh the screen
    refresh();

    // Handle user input
    int ch;
    while (true) {
        ch = getch();
        // Move cursor and update selection
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (cursorY > 3) {
                    mvprintw(cursorY, cursorX, " "); // Clear previous cursor
                    cursorY--;
                    mvprintw(cursorY, cursorX, ">"); // Print cursor at new position
                }
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                if (cursorY < 5) {
                    mvprintw(cursorY, cursorX, " "); // Clear previous cursor
                    cursorY++;
                    mvprintw(cursorY, cursorX, ">"); // Print cursor at new position
                }
                break;
            case 10: // Enter key
            case '1':
            case ' ':
                // Return the selected option's damage
                if (cursorY == 2)
                    return 5;
                else if (cursorY == 3)
                    return 10;
                else
                    return 20;
                break;
            case 27:
            case 8:
            case '2':
                return 0;
            default:
                break;
        }
    }
}

void info(){
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                << FLOOR INFO >>               -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print information
    mvprintw(3, 0, "-     PLAYER HEALTH        ---   100 HP         -");
    mvprintw(4, 0, "-     PLAYER ATTACK RANGE  ---   5-20 DMG       -");
    mvprintw(5, 0, "-     ENEMY HEALTH         ---   100 DMG        -");
    mvprintw(6, 0, "-     ENEMY ATTACK         ---   0 DMG          -");
    mvprintw(7, 0, "-                                               -");
    mvprintw(8, 0, "-------------------------------------------------");

    // Refresh the screen
    refresh();
}

void inventory(){
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                << INVENTORY >>                -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print inventory
    mvprintw(3, 0, "-      PLAYER HEALTH         ---   100 HP       -");
    mvprintw(4, 0, "-      SMALL POTION (10 HP)  ---   0            -");
    mvprintw(5, 0, "-      MED POTION (50 HP)    ---   0            -");
    mvprintw(6, 0, "-      BIG POTION (100 HP)   ---   0            -");
    mvprintw(7, 0, "-                                               -");
    mvprintw(8, 0, "-------------------------------------------------");

    // Refresh the screen
    refresh();
}

// Function to print the health bar
void printHealthBar(int health) {
    mvprintw(4, 16, "["); // Start of the health bar
    attron(COLOR_PAIR(1)); // Set color for the health bar
    for (int i = 0; i < health / 5; i++) {
        printw("="); // Print each segment of the health bar
    }
    attroff(COLOR_PAIR(1)); // Reset color
    for (int i = health / 5; i < 20; i++) {
        printw(" "); // Fill remaining space with empty characters
    }
    printw("]"); // End of the health bar
}

void playGameMenu2(int health){
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                 << FLOOR 1 >>                 -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Game initialization complete
    mvprintw(3, 0, "-               ENEMY: TEST DUMMY               -");
    mvprintw(4, 0, "-       HEALTH:                                 -");
    
    // Print the health bar
    printHealthBar(health);
    
    mvprintw(5, 0, "-                                               -");
    mvprintw(6, 0, "-                [    ATTACK   ]                -");
    mvprintw(7, 0, "-                [  INVENTORY  ]                -");
    mvprintw(8, 0, "-                [ INFORMATION ]                -");
    mvprintw(9, 0, "-                [     RUN     ]                -");
    mvprintw(10, 0, "-                                               -");
    mvprintw(11, 0, "-------------------------------------------------");

    // Refresh the screen
    refresh();
}

void playGameMenu1(){
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-             << CHOOSE YOUR PATH >>            -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print menu options
    mvprintw(3, 0, "-             [     EASY FLOOR    ]             -");
    mvprintw(4, 0, "-             [    NORMAL FLOOR   ]             -");
    mvprintw(5, 0, "-             [     HARD FLOOR    ]             -");
    mvprintw(6, 0, "-             [  CHECK INVENTORY  ]             -");
    mvprintw(7, 0, "-             [ BACK TO MAIN MENU ]             -");
    mvprintw(8, 0, "-                                               -");
    mvprintw(9, 0, "-------------------------------------------------");

    // Refresh the screen
    refresh();
}

void playGameS3(int difficulty){
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-               << ENEMY DROPS >>               -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print menu options
    mvprintw(3, 0, "-              +1 [ SMALL POTION ]              -");
    mvprintw(4, 0, "-             +1 [ STICK -- 1 DMG ]             -");
    mvprintw(5, 0, "-                                               -");
    mvprintw(6, 0, "-------------------------------------------------");

    mvprintw(8, 0, "That's the end of the prototype! Feel free to exit the game anytime.");

    // Refresh the screen
    refresh();
}

void playGameS2(int difficulty){
    int enemyHealth = 100;   // NOTE: DETERMINE BY DIFFICULY AND FLOOR (player and enemy data into structs)
    int damage = 0;
    
    playGameMenu2(enemyHealth);

    // Initialize cursor position
    int cursorY = 6;
    int cursorX = 17;

    // Set initial selection
    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 15, A_REVERSE, 0, NULL); // Highlight the whole option

    refresh();

    // Handle user input
    while (true) {
        int ch = getch();

        // Move cursor and update selection
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (selectedOption > 0) {
                    mvchgat(cursorY + selectedOption, cursorX, 15, A_NORMAL, 0, NULL); // Unhighlight previous option
                    selectedOption--;
                    mvchgat(cursorY + selectedOption, cursorX, 15, A_REVERSE, 0, NULL); // Highlight new option
                }
                break;

            case KEY_DOWN:
            case 's':
            case 'S':
                if (selectedOption < 3) {
                    mvchgat(cursorY + selectedOption, cursorX, 15, A_NORMAL, 0, NULL); // Unhighlight previous option
                    selectedOption++;
                    mvchgat(cursorY + selectedOption, cursorX, 15, A_REVERSE, 0, NULL); // Highlight new option
                }
                break;
            case 10: // Enter key
            case '1':
            case ' ':
                if (selectedOption == 0){
                    damage = attack();
                    enemyHealth -= damage;
                    if (enemyHealth <= 0){
                        playGameS3(difficulty);
                        while (true) {
                        int input = getch();
                        if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                            break;
                        }
                        return;
                    }
                    playGameMenu2(enemyHealth);
                }
                else if (selectedOption == 1){
                    inventory();
                    while (true) {
                        int input = getch();
                        if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                            break;
                    }
                    playGameMenu2(enemyHealth);
                }
                else if (selectedOption == 2){
                    info();
                    while (true) {
                        int input = getch();
                        if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                            break;
                    }
                    playGameMenu2(enemyHealth);
                }
                else if (selectedOption == 3){
                    playGameMenu1();
                    return;
                }
                break;
            default:
                break;
        }
    }
}

void playGameS1(int state){
    if (state == 1){
        // LOAD SAVE FILE
    }
    
    playGameMenu1();

    // Initialize cursor position
    int cursorY = 3;
    int cursorX = 14;

    // Set initial selection
    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 21, A_REVERSE, 0, NULL); // Highlight the whole option

    // Refresh the screen
    refresh();

    // Handle user input
    while (true) {
        int ch = getch();

        // Move cursor and update selection
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (selectedOption > 0) {
                    mvchgat(cursorY + selectedOption, cursorX, 21, A_NORMAL, 0, NULL);
                    selectedOption--;
                    mvchgat(cursorY + selectedOption, cursorX, 21, A_REVERSE, 0, NULL);
                }
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                if (selectedOption < 4) {
                    mvchgat(cursorY + selectedOption, cursorX, 21, A_NORMAL, 0, NULL);
                    selectedOption++;
                    mvchgat(cursorY + selectedOption, cursorX, 21, A_REVERSE, 0, NULL);
                }
                break;
            case 10: // Enter key
            case '1':
            case ' ':
                // Process selected game mode
                switch (selectedOption) {
                    case 0:
                        // Selected EASY FLOOR
                        playGameS2(selectedOption);
                        playGameMenu1();
                        break;
                    case 1:
                        // Selected NORMAL FLOOR
                        playGameS2(selectedOption);
                        playGameMenu1();
                        break;
                    case 2:
                        // Selected HARD FLOOR
                        playGameS2(selectedOption);
                        playGameMenu1();
                        break;
                    case 3:
                        // Selected CHECK INVENTORY
                        inventory();
                        while (true) {
                            int input = getch();
                            if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                                break;
                        }
                        playGameMenu1();
                        break;
                    case 4:
                        // Selected BACK TO MAIN MENU
                        return;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

// Function to print the loading bar
void printLoadingBar(int progress) {
    // Clear the loading bar area
    mvprintw(3, 0, "-                                               -"); // Clear previous progress
    mvprintw(3, 13, "Progress: [%3d%% ", progress); // Print updated progress

    // Calculate the number of '#' characters to represent progress
    int numBar = (progress * 10) / 100; // Max 10 hashes for 100%

    // Print the loading bar
    for (int i = 0; i < numBar; i++) {
        mvprintw(3, 27 + i, "=");
    }
    for (int i = numBar; i < 10; i++) {
        mvprintw(3, 27 + i, " ");
    }
    printw("]"); // End of the health bar
}

void newGame(){
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-             << CREATING NEW SAVE >>           -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Initialize variables
    int totalSteps = 5; // Total steps in the initialization process
    int progress = 0;   // Current progress

    // Loop through the initialization process
    for (int step = 1; step <= totalSteps; step++) {
        // Simulate some processing time for each step
        usleep(500000); // Sleep for 0.5 seconds (500 milliseconds)

        // Update progress and display loading bar
        progress = (step * 100) / totalSteps;
        printLoadingBar(progress);

        // Refresh the screen
        refresh();
    }

    mvprintw(4, 0, "-                                               -");
    mvprintw(5, 0, "-        Welcome to the dungeon challenge!      -");
    mvprintw(6, 0, "-    Each floor will give you three different   -");
    mvprintw(7, 0, "-   difficulty options where you will face an   -");
    mvprintw(8, 0, "-      enemy. For this milestone prototype,     -");
    mvprintw(9, 0, "-   the enemy cannot attack you, so choose any  -");
    mvprintw(10, 0, "-    difficulty and use the [ ATTACK ] option   -");
    mvprintw(11, 0, "-      until you successfully defeated them!    -");
    mvprintw(12, 0, "-                                               -");

    // Game initialization complete
    mvprintw(13, 0, "-            [ PLAY ]        [ QUIT ]           -");
    mvprintw(14, 0, "-                                               -");
    mvprintw(15, 0, "-------------------------------------------------");

    // Initialize cursor position
    int cursorY = 13;
    int cursorX = 13;

    // Set initial selection
    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 8, A_REVERSE, 0, NULL); // Highlight the whole option

    // Refresh the screen
    refresh();

    // Handle user input
    while (true) {
        int ch = getch();

        // Move cursor and update selection
        switch (ch) {
            case KEY_LEFT:
            case 'a':
            case 'A':
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (cursorX == 13) { // Toggle between YES and NO
                    mvchgat(cursorY, cursorX, 8, A_NORMAL, 0, NULL);
                    cursorX = 29;
                    selectedOption = (selectedOption + 1) % 2;
                } else {
                    mvchgat(cursorY, cursorX, 8, A_NORMAL, 0, NULL);
                    cursorX = 13;
                    selectedOption = (selectedOption + 1) % 2;
                }
                mvchgat(cursorY, cursorX, 8, A_REVERSE, 0, NULL);
                break;
            case 10: // Enter key
            case '1':
            case ' ':
                if (selectedOption == 0){
                    playGameS1(0);
                }
                return;
                break;
            case 27:
            case 8:
            case '2':
                return;
            default:
                break;
        }
    }
}

void loadGame(){

}

// Function to print the leaderboard menu
// NOTE: SAVE TO TXT FILE WITH NAME, FLOOR, AND DEATH BOOL FOR SAVE FILES AND RECORDS
void printLeaderboard() {
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-               << LEADERBOARD >>               -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print leaderboard content (replace with actual leaderboard data)
    mvprintw(3, 0, "-   1.      PLAYER     ---     FLOOR 0          -");
    mvprintw(4, 0, "-   2.      PLAYER     ---     FLOOR 0          -");
    mvprintw(5, 0, "-   3.      PLAYER     ---     FLOOR 0          -");
    mvprintw(6, 0, "-                                               -");
    mvprintw(7, 0, "-------------------------------------------------");

    // Refresh the screen
    refresh();
}

// Function to print the controls menu with highlighted current option
void printControlsMenu() {
    clear();

    // Print title
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                 << CONTROLS >>                -");
    mvprintw(2, 0, "-                                               -");
    attroff(COLOR_PAIR(1));

    // Print controls
    mvprintw(3, 0, "-   Cursor  ---  WASD / Arrow Keys              -");
    mvprintw(4, 0, "-   Select  ---  Space Bar / Enter / NUMPAD 1   -");
    mvprintw(5, 0, "-   Back    ---  ESC / Backspace / NUMPAD 2     -");
    mvprintw(6, 0, "-                                               -");
    mvprintw(7, 0, "-------------------------------------------------");

    // Refresh the screen
    refresh();
}

// Function to display the quit screen and get user confirmation
bool askQuitScreen() {
    clear();

    // Print quit screen message
    mvprintw(0, 0, "-------------------------------------------------");
    mvprintw(1, 0, "-                << CLOSE GAME >>               -");
    mvprintw(2, 0, "-                                               -");

    // Print menu options
    mvprintw(3, 0, "-               [ YES ]    [ NO ]               -");
    mvprintw(4, 0, "-                                               -");
    mvprintw(5, 0, "-------------------------------------------------");

    // Initialize cursor position
    int cursorY = 3;
    int cursorX = 16;

    // Set initial selection
    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 7, A_REVERSE, 0, NULL); // Highlight the whole option

    // Refresh the screen
    refresh();

    // Handle user input
    while (true) {
        int ch = getch();

        // Move cursor and update selection
        switch (ch) {
            case KEY_LEFT:
            case 'a':
            case 'A':
                if (cursorX == 27) {
                    mvchgat(cursorY, cursorX, 6, A_NORMAL, 0, NULL);
                    cursorX = 16;
                    selectedOption = 0;
                    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (cursorX == 16) {
                    mvchgat(cursorY, cursorX, 6, A_NORMAL, 0, NULL);
                    cursorX = 27;
                    selectedOption = 1;
                    mvchgat(cursorY, cursorX, 6, A_REVERSE, 0, NULL);
                }
                break;
            case 10: // Enter key
            case '1':
            case ' ':
                return selectedOption == 0; // Return true for "YES", false for "NO"
                break;
            case 27:
            case 8:
            case '2':
                return selectedOption = 0;
            default:
                break;
        }
    }
}

int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Initialize colors
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    // Main loop
    int ch;
    int selectedOption = 0;
    int playCheck = -1;
    bool running = true;
    while (running) {
        // Print the menu with highlighted current option
        printMenu(selectedOption);

        // Get user input
        ch = getch();

        // Handle user input
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                // Move the cursor up
                selectedOption = (selectedOption - 1 + 4) % 4;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                // Move the cursor down
                selectedOption = (selectedOption + 1) % 4;
                break;
            case 10: // Enter key
            case '1':
            case ' ':
                // Handle selection based on the current option
                switch (selectedOption) {
                    case 0:
                        playCheck = playMenu();
                        if (playCheck == 0)
                            newGame();
                        else if (playCheck == 1)
                            loadGame();
                        break;
                    case 1:
                        // Print leaderboard
                        printLeaderboard();
                        while (true) {
                            int input = getch();
                            if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                                break;
                        }
                        break;
                    case 2:
                       // Print controls menu
                        printControlsMenu();
                        // Wait for Esc, Backspace, or Numpad 2 key to return to main menu// Wait for Esc key to return to main menu
                        while (true) {
                            int input = getch();
                            if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                                break;
                        }
                        break;
                    case 3:
                        // Ask for confirmation to quit
                        if (askQuitScreen())
                            running = false;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    // Cleanup ncurses
    endwin();

    return 0;
}