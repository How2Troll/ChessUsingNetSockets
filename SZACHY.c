#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char rows[8] = {'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A'};

char board[8][8] = {
    {'w', 's', 'g', 'k', 'q', 'g', 's', 'w'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'W', 'S', 'G', 'Q', 'K', 'G', 'S', 'W'}};

char boardOneD[64] = {
    'w', 's', 'g', 'k', 'q', 'g', 's', 'w',
    'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
    '-', '-', '-', '-', '-', '-', '-', '-',
    '-', '-', '-', '-', '-', '-', '-', '-',
    '-', '-', '-', '-', '-', '-', '-', '-',
    '-', '-', '-', '-', '-', '-', '-', '-',
    'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
    'W', 'S', 'G', 'Q', 'K', 'G', 'S', 'W'};

void displayOneD()
{
    int counter = 0;
    printf("\n\n\n");
    for (int i = 0; i < sizeof(boardOneD); i++)
    {

        if (counter == 8)
        {
            printf("\n");
            counter = 0;
        }
        printf("%c ", boardOneD[i]);
        ++counter;
    }
    printf("\n\n\n");
}

void chess_game(char player[])
{
    int ValueStart = (player[0] - 65) * 8 + (player[1] - 48) - 1; // - 48 bo zero
    int ValueEnd = (player[3] - 65) * 8 + (player[4] - 48) - 1;   // - 48 bo zero
    int figura = boardOneD[ValueStart]; //pobranie figury z pola startowego
    boardOneD[ValueStart] = 45; //wyzerowanie pola startowego
    boardOneD[ValueEnd] = figura; //ustawianei na polu pola startowego
    displayOneD();
}

void display()
{
    int i, j, k;

    printf("\n");
    printf(" ");
    // indeks liczbowy
    for (i = 0; i < 8; i++)
    {
        printf("   %d", i + 1);
    }
    printf("\n");

    for (k = 0; k < 8; k++)
    {
        printf("  ");
        for (i = 0; i < 33; i++)
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
    for (i = 0; i < 33; i++)
    {
        printf("-");
    }

    printf("\n");
}

int main()
{
    char sourceMove[7], destMove[3];
    while (1)
    {
        // display();
        printf("\nPodaj pole pionka ktorym chcesz wykonac ruch i jego pole docelowe <np. A0 B4>:\n");
        fgets(sourceMove, sizeof(sourceMove), stdin);
        printf("Ruch:");
        fputs(sourceMove, stdout);
        chess_game(sourceMove);
    }
}
