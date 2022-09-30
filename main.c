#include "stdio.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <string.h>
#include <stdbool.h>
#include <libgen.h>
static bool stop = false;

static void handle_term(int sig)
{
    stop = true;
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, handle_term);

    if (argc <= 3)
    {
        printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);

    int sock = socket(PF_INET, SOCK_STREAM, 0); //PF_INET指使用ip建立socket
    //AF=address family
    //PF=protocol family
    assert(sock >= 0);

    struct sockaddr_in address;
    bzero(&address, sizeof(address)); //将address的地址空间全部清0
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr); //将数字ip地址转成二进制格式给address.sin_addr赋值
    address.sin_port = htons(port);            //转换数字格式的断开给address.sin_port

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, backlog);
    assert(ret != -1);

    int ppid = getpid();
    for (;;)
    {
        struct sockaddr_in client;
        socklen_t client_addrlength = sizeof(client);

        int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlength);

        // printf("get fd:%d\n", connfd);
        if (connfd < 0)
        {
            continue;
        }
        int pid = getpid();
        int subpid = fork();
        if (pid == subpid)
        {
            continue;
        }
        char buffer[4];
        memset(buffer, '\0', 4);
        char remote[INET_ADDRSTRLEN];
        printf("connected with ip: %s and port: %d\n",
               inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN), ntohs(client.sin_port));
        ret = recv(connfd, buffer, 4 - 1, 0);
        printf("pid%d,subpid:%d,get %d bytes data:%s\n", pid, subpid, ret, buffer);
        send(connfd, (void *)&buffer, 1, 0);
        close(connfd);
    }

    return 0;
}
