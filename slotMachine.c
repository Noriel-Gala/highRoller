//SLOT MACHINE Final Project, Noriel Gala, 11/21/2024, COP 2270 C for Engineers
//Originally I tried to use #include <conio.h> but modern compilers don't support that old library anymore so I had to opt for other libs
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //for system("cls") & sleep
#include <time.h>
#include <conio.h> //for _kbhit() and _getch()

void gotoxy(int x, int y) { //AI used for gotoxy
    COORD coord = {x -1, y - 1}; //0-based indexing
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Centers text on console screen (AI)
void printCentered(const char *text, int y) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int textLength = strlen(text);
    int x = (consoleWidth - textLength) / 2;
    gotoxy(x, y);
    printf("%s", text);
}

int main()
{
    int a, b, c;
    char playAgain;
    
    do {
    srand((unsigned int)time(NULL));
    
    system("cls");
    printf("\n\t\t***-: press any key to play the game :-***", 10);
    
    while (!_kbhit()) { //Wait for key press
        Sleep(100);
    }   
    getch(); //Consume pressed key
        
    system("cls"); //clears screen

    PlaySound("spinning.wav", NULL, SND_ASYNC | SND_FILENAME); //Play spinning sound effects

    DWORD startTime = GetTickCount(); //Record start time, AI
    DWORD elapsedTime = 0; //AI
        
    while (elapsedTime < 5000) { //Spin for 6 seconds, used AI for the elapsedTime & DWORD to set up automatic spinning, it was manual before
        Sleep(80);
        //generate random numbers
        a = rand() % 10;
        b = rand() % 10;
        c = rand() % 10;
        
        //display values centered (AI)
        char numbers[20];
        sprintf(numbers, "%d %d %d", a, b, c);
        printCentered(numbers, 12);
        fflush(stdout); //ensures output is updated immediately

        elapsedTime = GetTickCount() - startTime; //Calculate elapsed time
        
    }
    PlaySound(NULL, 0, SND_PURGE); //stops spinning sound after 6 seconds
    
    //Above sets up the classic arcade style machine with all the numbers spinning
    //Theme to be adjusted later this is just a prototype

    
    printf("\n\t\t");
    if (a == b && b == c) {
        PlaySound("jackpot.wav", NULL, SND_ASYNC | SND_FILENAME);
        printf("\n***-: You Won The Game! :-***", 12);

    } else {
        printf("\n***-: You Lost The Game. :-***\n", 12);
    }

    //Ask if the user wants to play again (AI)
    printf("\nWould you like to play again? (y/n): ", 14);
    playAgain = _getch(); //replay loop
    
    printf("\n"); //next line for clarity


} while (playAgain == 'y' || playAgain == 'Y'); // continue if user presses y or Y

printf("Thank you for playing!\n", 12);
return 0;
}