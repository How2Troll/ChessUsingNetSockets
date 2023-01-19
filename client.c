// #include	"unp.h"
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

void str_cli(FILE *fp, int sockfd);

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
    servaddr.sin6_port = htons(7); /* echo server */
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

    str_cli(stdin, sockfd); /* do it all */

    fprintf(stderr, "OK\n");
    fflush(stderr);

    exit(0);
}


void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    printf("Enter text:");

    bzero(recvline, sizeof(recvline));
    bzero(sendline, sizeof(sendline));

    while (fgets(sendline, MAXLINE, fp) != NULL)
    {

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
