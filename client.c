#include <sys/types.h>  /* basic system data types */
#include <sys/socket.h> /* basic socket definitions */
#include <sys/time.h>   /* timeval{} for select() */
#include <time.h>       /* timespec{} for pselect() */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>  /* inet(3) functions */
#include <errno.h>
#include <fcntl.h> /* for nonblocking */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 100
#define SA struct sockaddr



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
    printf("|\n  ---------------------------------\n\n");
}

void chess_game(char player[])
{
    int ValueStart = (player[0] - 65) * 8 + (player[1] - 48) - 1; // - 48 bo zero
    int ValueEnd = (player[3] - 65) * 8 + (player[4] - 48) - 1;   // - 48 bo zero
    int figura = board[ValueStart];                               // pobranie figury z pola startowego
    board[ValueStart] = 45;                                       // wyzerowanie pola startowego 45 to wartosc znaku '-'
    board[ValueEnd] = figura;                                     // ustawianie na polu pola startowego
}

void white_figures(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    bzero(recvline, sizeof(recvline));
    bzero(sendline, sizeof(sendline));
    printf("\n");
    display();
    printf("You're playing as white (which are capital letters)!\n");

    int x = 0;

    while (1)
    {
        if (x != 1)
        {
            printf("Podaj pole pionka ktorym chcesz wykonac ruch i jego pole docelowe <np. A0 B4>:\n");
        }
        x++;
        fgets(sendline, MAXLINE, fp);

        if (sendline[1] >= 48 && sendline[1] <= 57) // garbage handler
        {
            printf("SENDLINE = %s \n", sendline);
            chess_game(sendline);
            system("clear");
            display();
            printf("Wykonales posuniecie! Czekaj na ruch przeciwnika\n");
        }

        write(sockfd, sendline, strlen(sendline));

        if (read(sockfd, recvline, MAXLINE) == 0)
        {
            printf("Rozlaczono z serwerem. Osiagnieto maksymalna ilosc graczy\n");
            exit(0);
        }
        fputs(recvline, stdout);
        if (recvline[1] >= 48 && recvline[1] <= 57)
        {
            chess_game(recvline);
            system("clear");
            display();
            printf("Drugi gracz wykonal posuniecie!\n");
        }
        bzero(recvline, sizeof(recvline));
        bzero(sendline, sizeof(sendline));
    }
}

void black_figures(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    bzero(recvline, sizeof(recvline));
    bzero(sendline, sizeof(sendline));

    printf("\n");
    display();
    printf("You're playing as black (which are lower case)!\nWait for start from second player!\n");

    while (1)
    {

        if (read(sockfd, recvline, MAXLINE) == 0)
        {
            printf("Rozlaczono z serwerem. Osiagnieto maksymalna ilosc graczy\n");
            exit(0);
        }

        if (recvline[1] >= 48 && recvline[1] <= 57)
        {
            chess_game(recvline);
            system("clear");
            display();
            printf("Drugi gracz wykonal posuniecie! Pora na ciebie!\n");
            printf("Podaj pole pionka ktorym chcesz wykonac ruch i jego pole docelowe <np. A0 B4>:\n");
        }

        bzero(sendline, sizeof(sendline));
        fgets(sendline, MAXLINE, fp);
        if (sendline[1] >= 48 && sendline[1] <= 57) // garbage handler
        {
            chess_game(sendline);
            system("clear");
            display();
            printf("Wykonales posuniecie! Czekaj na ruch przeciwnika\n");
        }

        write(sockfd, sendline, strlen(sendline));

        bzero(recvline, sizeof(recvline));
        bzero(sendline, sizeof(sendline));
    }
}

// koniec szachow

int main(int argc, char **argv)
{
    int sockfd, n;
    struct sockaddr_in6 servaddr;
    char recvline[MAXLINE + 1];

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <IPv6_address> : \n", argv[0]);
        return 1;
    }
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket error : %s\n", strerror(errno));
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(20);
    if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0)
    {
        fprintf(stderr, "inet_pton error for %s : \n", argv[1]);
        return 1;
    }
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        fprintf(stderr, "connect error : %s \n", strerror(errno));
        return 1;
    }
    int myNum;

    printf("Choose side:\n1- White pieces\n2- Black pieces\n");
    scanf("%d", &myNum);

    if (myNum == 1)
    {   
        system("clear");
        printf("Playing as white\n\n");
        white_figures(stdin, sockfd); /* do it all */
    }
    else if (myNum == 2)
    {
        system("clear");
        printf("Playing as black\n");
        black_figures(stdin, sockfd); /* do it all */
    }
    else
    {
        printf("Erorr\n\n");
    }

    fprintf(stderr, "OK\n");
    fflush(stderr);

    exit(0);
}
