#include <ncurses.h>
#include <unistd.h>

void printMenu(int selectedOption) {
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< PLACEHOLDER TITLE >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "[ PLAY ]");
    mvprintw(3, 0, "[ LEADERBOARD ]");
    mvprintw(4, 0, "[ CONTROLS ]");
    mvprintw(5, 0, "[ QUIT ]");

    attron(A_REVERSE);
    switch (selectedOption) {
        case 0:
            mvprintw(selectedOption + 2, 0, "[ PLAY ]");
            break;
        case 1:
            mvprintw(selectedOption + 2, 0, "[ LEADERBOARD ]");
            break;
        case 2:
            mvprintw(selectedOption + 2, 0, "[ CONTROLS ]");
            break;
        case 3:
            mvprintw(selectedOption + 2, 0, "[ QUIT ]");
            break;
        default:
            break;
    }
    attroff(A_REVERSE);

    refresh();
}

int playMenu() {
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< PLAY >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "[ NEW GAME ]");
    mvprintw(3, 0, "[ LOAD SAVE ]");

    attron(A_REVERSE);
    mvprintw(2, 0, "[ NEW GAME ]");
    attroff(A_REVERSE);

    refresh();

    int ch;
    int selectedOption = 0;
    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (selectedOption > 0) {
                    mvprintw(selectedOption + 2, 0, "[ LOAD SAVE ]");
                    selectedOption--;
                    attron(A_REVERSE);
                    mvprintw(selectedOption + 2, 0, "[ NEW GAME ]");
                    attroff(A_REVERSE);
                }
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                if (selectedOption < 1) {
                    mvprintw(selectedOption + 2, 0, "[ NEW GAME ]");
                    selectedOption++;
                    attron(A_REVERSE);
                    mvprintw(selectedOption + 2, 0, "[ LOAD SAVE ]");
                    attroff(A_REVERSE);
                }
                break;
            case 10:
            case '1':
            case ' ':
                return selectedOption;
                break;
            default:
                break;
        }
    }
}

int attack(){
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< ATTACKS >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "  KICK    --- 5 DMG");
    mvprintw(3, 0, "  AXE     --- 10 DMG");
    mvprintw(4, 0, "  GUN     --- 20 DMG");

    int cursorY = 2;
    int cursorX = 0;

    attron(A_REVERSE);
    mvprintw(cursorY, cursorX, ">");
    attroff(A_REVERSE);

    refresh();

    int ch;
    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (cursorY > 2) {
                    mvprintw(cursorY, cursorX, " ");
                    cursorY--;
                    mvprintw(cursorY, cursorX, ">");
                }
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                if (cursorY < 4) {
                    mvprintw(cursorY, cursorX, " ");
                    cursorY++;
                    mvprintw(cursorY, cursorX, ">");
                }
                break;
            case 10:
            case '1':
            case ' ':
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

void inventory(){
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< INVENTORY >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "PLAYER HEALTH --- PLACEHOLDER");
    mvprintw(4, 0, "PLACEHOLDER   --- PLACEHOLDER");
    mvprintw(5, 0, "PLACEHOLDER   --- PLACEHOLDER");

    refresh();
}

void info(){
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< FLOOR INFO >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "PLAYER HEALTH       --- PLACEHOLDER");
    mvprintw(3, 0, "PLAYER ATTACK RANGE --- PLACEHOLDER");
    mvprintw(4, 0, "PLAYER DEFENSE      --- PLACEHOLDER");
    mvprintw(5, 0, "ENEMY HEALTH        --- PLACEHOLDER");
    mvprintw(6, 0, "ENEMY ATTACK        --- PLACEHOLDER");
    mvprintw(7, 0, "ENEMY DEFENSE       --- PLACEHOLDER");

    refresh();
}

void printHealthBar(int health) {
    mvprintw(4, 8, "[");
    attron(COLOR_PAIR(1));
    for (int i = 0; i < health / 5; i++) {
        printw("=");
    }
    for (int i = health / 5; i < 20; i++) {
        printw(" ");
    }
    printw("]");
}

void playGameMenu2(int health){
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< FLOOR PLACEHOLDER # >>");
    attroff(COLOR_PAIR(1));

    mvprintw(3, 0, "ENEMY (( NAME ))");
    mvprintw(4, 0, "HEALTH:");

    printHealthBar(health);

    mvprintw(6, 0, "[ ATTACK ]");
    mvprintw(6, 11, "[ INVENTORY ]");
    mvprintw(6, 25, "[ INFORMATION ]");
    mvprintw(6, 41, "[ RUN ]");

    refresh();
}

void playGameMenu1(){
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< CHOOSE YOUR PATH >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "[ EASY FLOOR ]");
    mvprintw(3, 0, "[ NORMAL FLOOR ]");
    mvprintw(4, 0, "[ HARD FLOOR ]");
    mvprintw(5, 0, "[ CHECK INVENTORY ]");
    mvprintw(6, 0, "[ BACK TO MAIN MENU ]");

    refresh();
}

void playGameS3(int difficulty){
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< ENEMY DROPS >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "[ PLACEHOLDER ]");
    mvprintw(3, 0, "[ PLACEHOLDER ]");
    mvprintw(4, 0, "[ PLACEHOLDER ]");
    mvprintw(5, 0, "[ PLACEHOLDER ]");
    mvprintw(6, 0, "[ PLACEHOLDER ]");

    mvprintw(8, 0, "That's the end of the prototype! Feel free to exit the game anytime.");

    refresh();
}

void playGameS2(int difficulty){
    // NOTE: DETERMINE BY DIFFICULY AND FLOOR (player and enemy data into structs)
    int enemyHealth = 100;
    int damage = 0;
    
    playGameMenu2(enemyHealth);

    int cursorY = 6;
    int cursorX = 0;

    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 8, A_REVERSE, 0, NULL);

    refresh();

    while (true) {
        attron(A_REVERSE);
        mvchgat(cursorY, cursorX, 10, A_NORMAL, 0, NULL);
        mvchgat(cursorY, cursorX, 10, A_REVERSE, 0, NULL);
        attroff(A_REVERSE);
        int ch = getch();

        switch (ch) {
            case KEY_LEFT:
            case 'a':
            case 'A':
                if (cursorX > 0) {
                    mvchgat(cursorY, cursorX, 10, A_NORMAL, 0, NULL);
                    cursorX -= 13;
                    selectedOption = (selectedOption - 1 + 4) % 4;
                    mvchgat(cursorY, cursorX, 10, A_REVERSE, 0, NULL);
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (cursorX < 35) {
                    mvchgat(cursorY, cursorX, 10, A_NORMAL, 0, NULL);
                    cursorX += 13;
                    selectedOption = (selectedOption + 1) % 4;
                    mvchgat(cursorY, cursorX, 10, A_REVERSE, 0, NULL);
                }
                break;
            case 10:
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

    attron(A_REVERSE);
    mvprintw(2, 0, "[ EASY FLOOR ]");
    attroff(A_REVERSE);

    int ch;
    int selectedOption = 0;
    while (true) {
        attron(A_REVERSE);
        mvprintw(selectedOption + 2, 0, "[ %s ]", selectedOption == 0 ? "EASY FLOOR" : (selectedOption == 1 ? "NORMAL FLOOR" : (selectedOption == 2 ? "HARD FLOOR" : (selectedOption == 3 ? "CHECK INVENTORY" : "BACK TO MAIN MENU"))));
        attroff(A_REVERSE);
        ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (selectedOption > 0) {
                    mvprintw(selectedOption + 2, 0, "[ %s ]", selectedOption == 1 ? "NORMAL FLOOR" : (selectedOption == 2 ? "HARD FLOOR" : (selectedOption == 3 ? "CHECK INVENTORY" : "BACK TO MAIN MENU")));
                    selectedOption--;
                    attron(A_REVERSE);
                    mvprintw(selectedOption + 2, 0, "[ %s ]", selectedOption == 0 ? "EASY FLOOR" : (selectedOption == 1 ? "NORMAL FLOOR" : (selectedOption == 2 ? "HARD FLOOR" : (selectedOption == 3 ? "CHECK INVENTORY" : "BACK TO MAIN MENU"))));
                    attroff(A_REVERSE);
                }
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                if (selectedOption < 4) {
                    mvprintw(selectedOption + 2, 0, "[ %s ]", selectedOption == 0 ? "EASY FLOOR" : (selectedOption == 1 ? "NORMAL FLOOR" : (selectedOption == 2 ? "HARD FLOOR" : (selectedOption == 3 ? "CHECK INVENTORY" : "BACK TO MAIN MENU"))));
                    selectedOption++;
                    attron(A_REVERSE);
                    mvprintw(selectedOption + 2, 0, "[ %s ]", selectedOption == 1 ? "NORMAL FLOOR" : (selectedOption == 2 ? "HARD FLOOR" : (selectedOption == 3 ? "CHECK INVENTORY" : "BACK TO MAIN MENU")));
                    attroff(A_REVERSE);
                }
                break;
            case 10:
            case '1':
            case ' ':
                switch (selectedOption) {
                    case 0:
                        playGameS2(selectedOption);
                        playGameMenu1();
                        break;
                    case 1:
                        playGameS2(selectedOption);
                        playGameMenu1();
                        break;
                    case 2:
                        playGameS2(selectedOption);
                        playGameMenu1();
                        break;
                    case 3:
                        inventory();
                        while (true) {
                            int input = getch();
                            if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                                break;
                        }
                        playGameMenu1();
                        break;
                    case 4:
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

void printLoadingBar(int progress) {
    mvprintw(2, 0, "                      ");
    mvprintw(2, 0, "Progress: [%3d%% ", progress);

    int numBar = (progress * 10) / 100;

    for (int i = 0; i < numBar; i++) {
        mvprintw(2, 14 + i, "=");
    }
    for (int i = numBar; i < 10; i++) {
        mvprintw(2, 14 + i, " ");
    }
    printw("]");
}

void newGame(){
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< CREATING NEW SAVE >>");
    attroff(COLOR_PAIR(1));

    int totalSteps = 5;
    int progress = 0;

    for (int step = 1; step <= totalSteps; step++) {
        usleep(500000);

        progress = (step * 100) / totalSteps;
        printLoadingBar(progress);

        refresh();
    }

    mvprintw(4, 0, "Welcome to the dungeon challenge! Each floor will give you three");
    mvprintw(5, 0, "different difficulty options where you will face an enemy.");
    mvprintw(6, 0, "For this milestone prototype, the enemy cannot attack you,");
    mvprintw(7, 0, "so choose any difficulty and use the [ ATTACK ] option until you");
    mvprintw(8, 0, "successfully defeated them!");

    mvprintw(10, 0, "[ PLAY ]");
    mvprintw(10, 10, "[ QUIT ]");

    int cursorY = 10;
    int cursorX = 0;

    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 8, A_REVERSE, 0, NULL);

    refresh();

    while (true) {
        int ch = getch();

        switch (ch) {
            case KEY_LEFT:
            case 'a':
            case 'A':
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (cursorX == 0) {
                    mvchgat(cursorY, cursorX, 8, A_NORMAL, 0, NULL);
                    cursorX = 10;
                    selectedOption = (selectedOption + 1) % 2;
                } else {
                    mvchgat(cursorY, cursorX, 8, A_NORMAL, 0, NULL);
                    cursorX = 0;
                    selectedOption = (selectedOption + 1) % 2;
                }
                mvchgat(cursorY, cursorX, 8, A_REVERSE, 0, NULL);
                break;
            case 10:
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
    // NOTE: IMPLEMENT LATER
}

// NOTE: SAVE TO TXT FILE WITH NAME, FLOOR, AND DEATH BOOL FOR SAVE FILES AND RECORDS
void printLeaderboard() {
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< LEADERBOARD >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "1. Player1 - Floor: ---");
    mvprintw(3, 0, "2. Player2 - Floor: ---");
    mvprintw(4, 0, "3. Player3 - Floor: ---");
    mvprintw(5, 0, "4. Player4 - Floor: ---");
    mvprintw(6, 0, "5. Player5 - Floor: ---");

    refresh();
}

void printControlsMenu() {
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "<< CONTROLS >>");
    attroff(COLOR_PAIR(1));

    mvprintw(2, 0, "Cursor Movement --- WASD or Arrow Keys");
    mvprintw(3, 0, "Select Option   --- Space Bar or Enter or NUMPAD 1");
    mvprintw(4, 0, "Go Back         --- ESC or Backspace or NUMPAD 2");

    refresh();
}

bool askQuitScreen() {
    clear();

    mvprintw(0, 0, "<< CLOSE GAME >>");

    mvprintw(2, 0, "[ YES ]");
    mvprintw(2, 10, "[ NO ]");

    int cursorY = 2;
    int cursorX = 0;

    int selectedOption = 0;
    mvchgat(cursorY, cursorX, 7, A_REVERSE, 0, NULL);

    refresh();

    while (true) {
        int ch = getch();

        switch (ch) {
            case KEY_LEFT:
            case 'a':
            case 'A':
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (cursorX == 0) {
                    mvchgat(cursorY, cursorX, 7, A_NORMAL, 0, NULL);
                    cursorX = 10;
                    selectedOption = (selectedOption + 1) % 2;
                } else {
                    mvchgat(cursorY, cursorX, 7, A_NORMAL, 0, NULL);
                    cursorX = 0;
                    selectedOption = (selectedOption + 1) % 2;
                }
                mvchgat(cursorY, cursorX, 7, A_REVERSE, 0, NULL);
                break;
            case 10:
            case '1':
            case ' ':
                return selectedOption == 0;
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
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    int ch;
    int selectedOption = 0;
    int playCheck = -1;
    bool running = true;
    while (running) {
        printMenu(selectedOption);

        ch = getch();

        switch (ch) {
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
                switch (selectedOption) {
                    case 0:
                        playCheck = playMenu();
                        if (playCheck == 0)
                            newGame();
                        else if (playCheck == 1)
                            newGame();
                            // NOTE: IMPLEMENT AT A LATER POINT
                            //loadGame();
                        break;
                    case 1:
                        printLeaderboard();
                        while (true) {
                            int input = getch();
                            if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                                break;
                        }
                        break;
                    case 2:
                        printControlsMenu();
                        while (true) {
                            int input = getch();
                            if (input == 27 || input == 8 || input == '2' || input == KEY_DC)
                                break;
                        }
                        break;
                    case 3:
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

    endwin();
    return 0;
}