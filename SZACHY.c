#include <stdlib.h>
#include <stdio.h>

char rows[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

char board[64] = {
    'w', 's', 'g', 'k', 'q', 'g', 's', 'w',
    'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
    '-', '-', '-', '-', '-', '-', '-', '-',
    '-', '-', '-', '-', '-', '-', '-', '-',
    '-', '-', '-', '-', '-', '-', '-', '-',
    '-', '-', '-', '-', '-', '-', '-', '-',
    'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
    'W', 'S', 'G', 'Q', 'K', 'G', 'S', 'W'};

void display()
{
    int counter = 0;
    int j = 0;
    printf("\n\n ");
    for (int i = 0; i < 8; i++)
    {
        printf("   %d", i + 1);
    }
    printf("\n  ---------------------------------\nA ");
    for (int i = 0; i < sizeof(board); i++)
    {

        if (counter == 8)
        {
            j++;
            printf("|\n  ---------------------------------\n%c ", rows[j]);
            counter = 0;
        }
        printf("| %c ", board[i]);
        ++counter;
    }
    printf("|\n  ---------------------------------\n\n\n");
}

void chess_game(char player[])
{
    int ValueStart = (player[0] - 65) * 8 + (player[1] - 48) - 1; // - 48 bo zero
    int ValueEnd = (player[3] - 65) * 8 + (player[4] - 48) - 1;   // - 48 bo zero
    int figura = board[ValueStart];                           // pobranie figury z pola startowego
    board[ValueStart] = 45;                                   // wyzerowanie pola startowego 45 to wartosc znaku '-'
    board[ValueEnd] = figura;       // ustawianie na polu pola startowego
    system ("clear");                                 
}

int main()
{
    char sourceMove[7];
    while (1)
    {
        display();
        printf("\nPodaj pole pionka ktorym chcesz wykonac ruch i jego pole docelowe <np. A0 B4>:\n");
        fgets(sourceMove, sizeof(sourceMove), stdin);
        printf("Ruch:");
        fputs(sourceMove, stdout);
        chess_game(sourceMove);
    }
}
