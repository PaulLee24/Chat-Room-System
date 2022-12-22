#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

#define MAX_LINE 1026

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        exit(1);
    }

    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    char sendline[MAX_LINE];
    char recvline[MAX_LINE];

    int n = read(sockfd, recvline, MAX_LINE);
    fputs(recvline, stdout);

    fd_set rset;
    int max_fd;
    int stdin_fd = fileno(stdin);

    FD_ZERO(&rset);

    while (1)
    {
        FD_SET(stdin_fd, &rset);
        FD_SET(sockfd, &rset);
        max_fd = (sockfd > stdin_fd ? sockfd : stdin_fd) + 1;
        select(max_fd, &rset, NULL, NULL, NULL);

        int n;
        if (FD_ISSET(stdin_fd, &rset))
        {
            n = read(stdin_fd, sendline, MAX_LINE);
            write(sockfd, sendline, n);
            //cout << n << endl;
        }

        if (FD_ISSET(sockfd, &rset))
        {
            n = read(sockfd, recvline, MAX_LINE);
            if (n == 0)
            {
                //cout << "The server has closed the connection." << endl;
                close(sockfd);
                return 0;
            }
            recvline[n] = '\0';
            fputs(recvline, stdout);
        }
    }
}