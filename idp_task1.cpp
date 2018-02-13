#include <iostream>
#include "util.h"

void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}

void err_quit(const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    int sockd, n;
    char receive_line [MAXLINE + 1];
    struct sockaddr_in addr;

    if(argc != 2)
        err_quit("usage : a.out <ip address>");

    if(sockd = socket(AF_INET, SOCK_STREAM,0) < 0)
        err_sys("socket error");

    bzero (&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(13);
    if(inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    if(connect(sockd, (const sockaddr*)&addr, sizeof(addr)) < 0)
        err_sys("connect error");

    while ((n = read(sockd, receive_line, MAXLINE))> 0)
    {
        receive_line[n] = 0;
        if(fputs(receive_line, stdout)== EOF)
        err_sys("fputs error");
    }

    if(n < 0)
        err_sys("read error");

    exit(0);
}