#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // for system("cls") & Sleep
#include <time.h>
#include <conio.h> // for _kbhit() and _getch()
#include <mmsystem.h> // for sound functions
#include <stdarg.h> // for variable arguments

void gotoxy(int x, int y) {
    COORD coord = {x - 1, y - 1}; // 0-based indexing
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printCentered(const char *format, int y, ...) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    va_list args;
    va_start(args, y);
    char buffer[1024]; // Buffer to hold the formatted string
    int textLength = vsprintf(buffer, format, args);
    va_end(args);

    int x = (consoleWidth - textLength) / 2;

    COORD coord = {x, y - 1}; // 0-based indexing
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    va_start(args, y);
    vprintf(format, args);
    va_end(args);
}

void setConsoleFullscreen() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD largestSize = GetLargestConsoleWindowSize(hOut);
    SMALL_RECT windowSize = {0, 0, largestSize.X - 1, largestSize.Y - 1};
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    SetConsoleScreenBufferSize(hOut, largestSize);
}

void displayMenu(const char *asciiArt[], int lines, int playerMoney) {
    system("cls");

    // Display High Roller title
    for (int i = 0; i < lines; i++) {
        printCentered(asciiArt[i], 5 + i);
    }

    // Display the player's current balance
    printCentered("Current balance: $%d", 11, playerMoney);

    // Display menu options
    printCentered("Press (1) to play normal mode", 14);
    printCentered("Press (2) to visit the shop", 15);
    printCentered("Press (3) to view stats", 16);
    printCentered("Press (q) to quit game (exit)", 17);
}

void displayShop(int playerMoney) {
    system("cls");
    printCentered("Welcome to the Shop!", 5);
    printCentered("1. Buy Luck Enhancer (+1 winning chance) - $10", 7);
    printCentered("Current balance: $%d", 9, playerMoney);
    printCentered("Press (b) to buy, (m) to return to the main menu", 11);
}

void playNormalMode(int *playerMoney, int *luckEnhancers, int *totalCashEarned, int *timesWon, int *moneySpent) {
    int a, b, c;
    char playAgain;
    float winChance = 0.01 + (*luckEnhancers * 0.01); // Base 1% chance, each enhancer adds 1% more

    // Display instructions to start the game
    system("cls");
    printCentered("\n\t\t***-: press any key to play the game :-***", 5);

    // Wait for a key press once and clear it
    while (!_kbhit()); // Wait for a key press
    _getch(); // Clear the key press from the buffer

    do {
        srand((unsigned int)time(NULL));

        system("cls");
        printCentered("\n\t\t***-: press any key to play the game :-***", 5);

        if (_kbhit()) {
            _getch(); // Clear any buffered input
        }

        system("cls");

        PlaySound("spinning.wav", NULL, SND_ASYNC | SND_FILENAME); // Play spinning sound effects

        DWORD startTime = GetTickCount();
        DWORD elapsedTime = 0;

        while (elapsedTime < 5000) { // Spin for 5 seconds
            Sleep(80);
            a = rand() % 10;
            b = rand() % 10;
            c = rand() % 10;

            char numbers[20];
            sprintf(numbers, "%d %d %d", a, b, c);
            printCentered(numbers, 12);
            fflush(stdout);

            elapsedTime = GetTickCount() - startTime;
        }
        PlaySound(NULL, 0, SND_PURGE);

        float winProbability = (float)rand() / RAND_MAX;
        if (winProbability < winChance) {
            *playerMoney += 1000; // Player wins 1,000x the buy-in
            *totalCashEarned += 1000;
            (*timesWon)++;
            PlaySound("jackpot.wav", NULL, SND_ASYNC | SND_FILENAME);
            printCentered("***-: You Won The Game! :-***", 12);
        } else {
            *playerMoney -= 1; // Deduct 1 dollar for the roll
            *moneySpent += 1;
            printCentered("***-: You Lost The Game. :-***", 12);
        }

        printCentered("\nWould you like to play again? (y/n) or return to main menu (m): ", 14);
        playAgain = _getch();

        if (playAgain == 'n' || playAgain == 'N') {
            printf("Exiting the game...\n");
            exit(0);
        } else if (playAgain == 'm' || playAgain == 'M') {
            printf("Returning to the main menu...\n");
            PlaySound("themesong.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            return;
        }

    } while (playAgain == 'y' || playAgain == 'Y');

    printf("Thank you for playing!\n");
}

void shop(int *playerMoney, int *luckEnhancers) {
    char choice;
    do {
        displayShop(*playerMoney);
        choice = _getch();

        if (choice == 'b' || choice == 'B') {
            if (*playerMoney >= 10) {
                *playerMoney -= 10;
                (*luckEnhancers)++;
                printf("Luck enhancer purchased! Current enhancers: %d\n", *luckEnhancers);
            } else {
                printf("Insufficient funds to buy a Luck Enhancer.\n");
            }
        } else if (choice == 'm' || choice == 'M') {
            printf("Returning to the main menu...\n");
            return;
        }
    } while (choice != 'm' && choice != 'M');
}

void displayStats(int totalCashEarned, int timesWon, int moneySpent) {
    system("cls");
    printCentered("Game Statistics", 5);
    printCentered("Total Cash Earned: $%d", 7, totalCashEarned);
    printCentered("Total Times Won: %d", 8, timesWon);
    printCentered("Total Money Spent: $%d", 9, moneySpent);
    printCentered("Press any key to return to the main menu", 11);
    _getch();
}


int main() {
    const char *asciiArt[] = {
        " _   _ _       _      ______      _ _           ",
        "| | | (_)     | |     | ___ \\    | | |          ",
        "| |_| |_  __ _| |__   | |_/ /___ | | | ___ _ __ ",
        "|  _  | |/ _` | '_ \\  |    // _ \\| | |/ _ \\ '__|",
        "| | | | | (_| | | | | | |\\ \\ (_) | | |  __/ |   ",
        "\\_| |_/_|\\__, |_| |_| \\_| \\\\___/|_|_|\\___|_|   ",
        "          __/ |                                 ",
        "         |___/                                  "
    };

    int playerMoney = 100;
    int luckEnhancers = 0;
    int totalCashEarned = 0;
    int timesWon = 0;
    int moneySpent = 0;

    system("mode con: cols=700 lines=700");
    keybd_event(VK_MENU, 0x38, 0, 0); // Press ALT
    keybd_event(VK_RETURN, 0x1C, 0, 0); // Press ENTER
    keybd_event(VK_RETURN, 0x1C, KEYEVENTF_KEYUP, 0); // Release ENTER
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0); // Release ALT

    setConsoleFullscreen();
    PlaySound("themesong.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

    while (1) {
        displayMenu(asciiArt, 7, playerMoney);
        char choice = _getch();

        if (choice == '1') {
            playNormalMode(&playerMoney, &luckEnhancers, &totalCashEarned, &timesWon, &moneySpent);
        } else if (choice == '2') {
            shop(&playerMoney, &luckEnhancers);
        } else if (choice == '3') {
            displayStats(totalCashEarned, timesWon, moneySpent);
        } else if (choice == 'q' || choice == 'Q') {
            printf("Quitting game...\n");
            exit(0);
        }
    }

    return 0;
}
