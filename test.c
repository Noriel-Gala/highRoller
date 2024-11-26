#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep() (Windows equivalent is Sleep() in <windows.h>)
#include <time.h>   // For time() and srand()

void playMP3(const char *filename) {
    char command[256];
    snprintf(command, sizeof(command), "open \"%s\" type mpegvideo alias mp3", filename);
    mciSendString(command, NULL, 0, NULL);
    mciSendString("play mp3", NULL, 0, NULL);
} //sets up the sound effect to play when you hit jackpot

void gotoxy(int x, int y) {
    // ANSI escape code to move the cursor
    printf("\033[%d;%dH", y, x);
}

int kbhit() {
    // Non-blocking check for key press
    struct timeval tv;
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(0, &readfds); // 0 refers to stdin

    tv.tv_sec = 0;
    tv.tv_usec = 100000; // 0.1 second timeout

    select(1, &readfds, NULL, NULL, &tv);

    return FD_ISSET(0, &readfds);
}

int main() {
    int a, b, c;

    // Seed the random number generator
    srand(time(NULL));

    printf("\n\t\t***-: Press any key to play the game :-***\n");

    // Wait for a key press to start
    while (!kbhit()) {}

    // Clear the screen and start the game
    printf("\033[2J"); // Clear the screen using ANSI code

    // Main game loop
    while (1) {
        usleep(80000); // Pause for 80 milliseconds

        // Generate random values for a, b, and c between 0 and 9
        a = rand() % 10;
        b = rand() % 10;
        c = rand() % 10;

        gotoxy(20, 10); // Move the cursor to position (20, 10)
        printf("%d %d %d", a, b, c); // Print the updated values
        fflush(stdout); // Ensure output is updated immediately

        if (kbhit()) { // Exit loop when a key is pressed
            getchar(); // Consume the pressed key
            break;
        }
    }

    // Check if the player won
    printf("\n\t\t");
    if (a == b && b == c) {
        playMP3("win_sound.mp3");
        printf("\n***-: You Won The Game! :-***\n");
    } else {
        printf("\n***-: You Lost The Game. :-***\n-> Try Again\n");
    }

    return 0;
}
