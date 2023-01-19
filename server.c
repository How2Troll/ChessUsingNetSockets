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
#include <sys/wait.h>

#define MAXLINE 1024

#define LISTENQ 2

// TO DO

// jak serwer rozpoznaje do kogo wyslac wiadomosc?
// teraz poznaje po sockecie

// jak serwer rozpoznaje od kogo odberal?

struct client
{
    struct sockaddr_in6 addr;
    int sockfd;
    int clientId;
};

void sig_chld(int signo) // funckja UBIJANAI dzieci
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

void str_echo(int sockfd1, int sockfd2) // komunikacja z klientem
{
    ssize_t n1, n2;
    char buf1[MAXLINE];
    char buf2[MAXLINE];

    while (1)
    {
        // w kliecnie write ->read
        // klient 1 tylko napisze to czeka na odbieranie danych
        // skoro klient 2 nic nie pisze to klient1 nie odbierze danych i nie wejdzie do write

        while (((n1 = read(sockfd1, buf1, MAXLINE)) > 0) && (n2 = read(sockfd2, buf2, MAXLINE)) > 0) // odibera, zwraca liczbe przeczytanych bajtow
        {

            if (strcmp(buf2, "exit\n") == 0)
            {
                close(sockfd2); // Close the existing socket which is connected to the client
                exit(0);
            }

            if (strcmp(buf1, "exit\n") == 0)
            {
                close(sockfd1); // Close the existing socket which is connected to the client
                exit(0);
            }

            write(sockfd2, buf1, n1); // tu pisze sockfd 4 buff
            write(sockfd1, buf2, n2);
            bzero(buf1, sizeof(buf1));
            bzero(buf2, sizeof(buf2));
        }

        if (n1 < 0 && errno == EINTR)
            continue;

        else if (n1 < 0)
            perror("str_echo: read error");
    }
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct client clients[2];
    struct sockaddr_in6 cliaddr, servaddr;
    void sig_chld(int);
    int userId = 0;

    signal(SIGCHLD, sig_chld);

    if ((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket error : %s\n", strerror(errno));
        return 1;
    }
    int optval = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        fprintf(stderr, "SO_REUSEADDR setsockopt error : %s\n", strerror(errno));
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_addr = in6addr_any;
    servaddr.sin6_port = htons(7); /* echo server */

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        fprintf(stderr, "bind error : %s\n", strerror(errno));
        return 1;
    }

    if (listen(listenfd, LISTENQ) < 0)
    {
        fprintf(stderr, "listen error : %s\n", strerror(errno));
        return 1;
    }

    while (1)
    {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
        {
            if (errno == EINTR)
                continue; /* back to while() */
            else
                perror("accept error");
            exit(1);
        }
        else
        {
            clients[userId].addr = cliaddr;
            clients[userId].sockfd = connfd;
            ++userId;
        }

        if (userId > 1)
        {
            if ((childpid = fork()) == 0)
            {                                                                     /* child process */
                close(listenfd);
                printf("sockfd1: %d\n", clients[userId - 2].sockfd);                                              /* close listening socket */
                printf("sockfd2: %d\n", clients[userId - 1].sockfd);
                printf("por1: %d\n", ntohs(clients[userId - 2].addr.sin6_port));
                printf("por2: %d\n", ntohs(clients[userId - 1].addr.sin6_port));
                str_echo(clients[userId - 2].sockfd, clients[userId - 1].sockfd); /* process the request */
                exit(0);
            }
            close(connfd); /* parent closes connected socket */
        }
    }
}