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

struct client
{
    struct sockaddr_in6 addr;
    int sockfd;
    int clientId;
};

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

void play(int sockfd1, int sockfd2) // komunikacja z klientem
{
    ssize_t n1, n2;
    char buf1[MAXLINE];
    char buf2[MAXLINE];
    int player = 1;
    bzero(buf1, sizeof(buf1));
    bzero(buf2, sizeof(buf2));

    while (1)
    {
        if (player == 1)
        {
            n1 = read(sockfd1, buf1, MAXLINE);

            if (strcmp(buf1, "exit\n") == 0)
            {
                close(sockfd1); // Close the existing socket which is connected to the client
                exit(0);
            }

            write(sockfd2, buf1, n1);
            bzero(buf1, sizeof(buf1));
            player = 2;
        }

        if (player == 2)
        {
            n2 = read(sockfd2, buf2, MAXLINE);

            if (strcmp(buf2, "exit\n") == 0)
            {
                close(sockfd2); // Close the existing socket which is connected to the client
                exit(0);
            }

            write(sockfd1, buf2, n2); // tu pisze sockfd 4 buff
            bzero(buf2, sizeof(buf2));
            player = 1;
        }
    }
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct client clients[2];
    struct sockaddr_in6 servaddr;
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
    servaddr.sin6_port = htons(20);

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
        clilen = sizeof(clients->addr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&clients->addr, &clilen)) < 0)
        {
            if (errno == EINTR)
                continue;
            else
                perror("accept error");
            exit(1);
        }
        else
        {
            clients[userId].addr = clients->addr;
            clients[userId].sockfd = connfd;
            ++userId;
        }

        if (userId > 1)
        {
            if ((childpid = fork()) == 0)
            {
                close(listenfd);
                printf("sockfd1: %d\n", clients[0].sockfd);
                printf("sockfd2: %d\n", clients[1].sockfd);
                printf("port1: %d\n", ntohs(clients[0].addr.sin6_port));
                printf("port2: %d\n", ntohs(clients[1].addr.sin6_port));
                play(clients[0].sockfd, clients[1].sockfd);
                exit(0);
            }
            close(clients[0].sockfd);
            close(clients[1].sockfd);
        }
    }
}