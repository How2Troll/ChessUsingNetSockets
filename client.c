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


//TO DO

//jak serwer

#define MAXLINE 1024
#define SA struct sockaddr

void str_cli_white(FILE *fp, int sockfd);
void str_cli_black(FILE *fp, int sockfd);

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
    servaddr.sin6_port = htons(20); /* echo server */
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

    printf("1- Black pieces\n2- White pieces\n");
    scanf("%d", &myNum);;

    if(myNum == 1)
    {
        printf("Playing as black\n\n");
        str_cli_black(stdin, sockfd); /* do it all */
    }
    else if(myNum == 2)
    {
        printf("Playing as white\n\n");
        str_cli_white(stdin, sockfd); /* do it all */
    }
    else
    {
        printf("Erorr\n\n");
    }


    fprintf(stderr, "OK\n");
    fflush(stderr);

    exit(0);
}

//bialy
//print szachownica
//fgets
//write
//read
void str_cli_white(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    bzero(recvline, sizeof(recvline));
    bzero(sendline, sizeof(sendline));

    while (1)
    {
        fgets(sendline, MAXLINE, fp);
        write(sockfd, sendline, strlen(sendline));

        if (read(sockfd, recvline, MAXLINE) == 0)
        {
            perror("str_cli: server terminated prematurely");
            exit(0);
        }

        fputs(recvline, stdout);
        printf("Enter text:");
		bzero(recvline, sizeof(recvline));
    	bzero(sendline, sizeof(sendline));
    }
}


//czorny
//read
//print szachownice
//fputs
//print szachownice
//fgets
//write
void str_cli_black(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    bzero(recvline, sizeof(recvline));
    bzero(sendline, sizeof(sendline));

    while (1)
    {
        if (read(sockfd, recvline, MAXLINE) == 0)
        {
            perror("str_cli: server terminated prematurely");
            exit(0);
        }
        fputs(recvline, stdout);
        printf("Enter text:");
        fgets(sendline, MAXLINE, fp);
        write(sockfd, sendline, strlen(sendline));

		bzero(recvline, sizeof(recvline));
    	bzero(sendline, sizeof(sendline));
    }
}