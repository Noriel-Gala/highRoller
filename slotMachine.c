//SLOT MACHINE Final Project, Noriel Gala, 11/21/2024, COP 2270 C for Engineers
//Originally I tried to use #include <conio.h> but modern compilers don't support that old library anymore so I had to opt for other libs
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //for system("cls") & sleep

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

int main()
{
    int a = 0, b = 0, c = 0;
    CLEAR_SCREEN();
    printf("\n\t\t***-: press any key to play the game :-***");
    getchar(); //Waits for key press
    while (1) { //replaces 'for (kbhit())' with an inf loop
        DELAY(80);
        a++;
        b++;
        c++;
        gotoxy(20, 10);
        printf("%d %d %d", (a-1), (b-1), (c-1));
        fflush(stdout); //AI, ensures output is updated immediately
        if (a == 10) {
            a = 0;
        } if (b == 10) {
            b = 4;
        } if (c == 10) {
            c = 0;
        }
        if (getchar()) break; //Exits loop upon key press
    } 
    //Above sets up the classic arcade style machine with all the numbers flowing down and spinning system
    //Theme to be adjusted later this is just a prototype

    printf("\n\t\t");
    if (a == b && b == c) {
        printf("\n***-: You Won The Game! :-***");

    } else {
        printf("\n***-: You Lost The Game. :-***\n-> Try Again\n");
    }
}