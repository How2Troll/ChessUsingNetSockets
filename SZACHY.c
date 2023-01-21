#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char rows[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

char board[8][8] = {
    {'w', 's', 'g', 'k', 'q', 'g', 's', 'w'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'W', 'S', 'G', 'Q', 'K', 'G', 'S', 'W'}};

void display()
{
    int i, j, k;

    printf("\n");
    printf(" ");
    for (i = 0; i < 8; i++)
    {
        printf("   %d", i);
    }
    printf("\n");

    for (k = 0; k < 8; k++)
    {
        printf("  ");
        for (i = 0; i < 40; i++)
        {
            printf("-");
        }

        printf("\n");
        printf("%c ", rows[k]);
        for (j = 0; j < 8; j++)
        {
            printf("| %c ", board[k][j]);
        }

        printf("| \n");
    }

    printf("  ");
    for (i = 0; i < 42; i++)
    {
        printf("-");
    }

    printf("\n");
}


int main()
{
    char sourceMove[7], destMove[3];
    while(1){
    display();
    printf("\nPodaj pole pionka ktorym chcesz wykonac ruch i jego pole docelowe <np. A0 B4>:\n");
    fgets(sourceMove, sizeof(sourceMove), stdin);
    printf("Ruch:");
    fputs(sourceMove, stdout);
    }
}

    // char name[30];
    // printf("Enter name: ");
    // fgets(name, sizeof(name), stdin);  // read string
    // printf("Name: ");
    // puts(name);    // display string
    // return 0;


