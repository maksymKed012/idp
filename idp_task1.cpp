#include <iostream>
#include "util.h"

static const int LISTENQ = 50;
static const int DEFAULT_ERROR = -1;
static const int SERVER_PORT = 8877;                                                           

//error handlers
void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}
                                                        
void err_quit(const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  printf(fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

//Wrapper functions
int Socket(int domain, int type, int protocol)
{
    CALL_FUNC_QUIT_ON_ERR(socket, DEFAULT_ERROR, domain, type, protocol);
}

int initInAddrForIP(struct in_addr* addr, char* ip_address)
{
    CALL_FUNC_QUIT_ON_ERR(inet_pton, NULL, AF_INET, ip_address, addr);
}

int Bind(int listen_file_descr, struct sockaddr* socket_address, size_t size)
{
    CALL_FUNC_QUIT_ON_ERR(bind, DEFAULT_ERROR, listen_file_descr, socket_address, size)
}

int Listen(int connection_handle, int flag)
{
    CALL_FUNC_QUIT_ON_ERR(listen, DEFAULT_ERROR, connection_handle, flag);
}

int Accept(int handle, struct sockaddr* socket_address, socklen_t* socket_length)
{
    CALL_FUNC_QUIT_ON_ERR(accept, DEFAULT_ERROR, handle, socket_address, socket_length);
}

int Write(int handle, char* buffer, size_t buffer_size)
{
    CALL_FUNC_QUIT_ON_ERR(write, DEFAULT_ERROR, handle, buffer, buffer_size);
}
    
int Close(int socket_handle)
{
    CALL_FUNC_QUIT_ON_ERR(close, DEFAULT_ERROR, socket_handle);
}

int Send(int socket_handle, const void* buffer, size_t buffer_len, int flags)
{
    CALL_FUNC_QUIT_ON_ERR(send, DEFAULT_ERROR, socket_handle, buffer, buffer_len, flags);
}

void initSockAddr(struct sockaddr_in* sockaddr, int family, int address, int port)
{
    if(!sockaddr)
        err_quit("initSockAddr - sockaddr is NULL");

    bzero(sockaddr, sizeof(*sockaddr));
    sockaddr->sin_family = family;
    sockaddr->sin_addr.s_addr = address;
    sockaddr->sin_port = port;
}

void createTestTCPClient()
{
    int sockd, n;
    char receive_line [MAXLINE + 1];
    struct sockaddr_in addr;
    const char* ip_address = "128.138.141.172";

    if((sockd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    bzero (&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(13);
    if((inet_pton(AF_INET, ip_address, &addr.sin_addr)) <= 0)
        err_quit("inet_pton error for %s", ip_address);

    if((connect(sockd, (struct sockaddr*)&addr, sizeof(addr))) < 0)
        err_sys("connect error");

    while ((n = read(sockd, receive_line, MAXLINE))> 0)
    {
        receive_line[n] = 0;
        if(fputs(receive_line, stdout) == EOF)
            err_sys("fputs error");
    }

    if(n < 0)
        err_sys("read error");

    exit(0);
}

void createTestTCPServer()
{
    int socket_desc;
    int connection_handle;
    struct sockaddr_in socket_address;

    socket_desc = Socket(AF_INET, SOCK_STREAM, 0);

    initSockAddr(&socket_address, AF_INET, htonl(INADDR_ANY), htons(SERVER_PORT));

    Bind(socket_desc, (struct sockaddr*)&socket_address, sizeof(socket_address));

    Listen(socket_desc, LISTENQ);

    struct sockaddr_in client_address;
	socklen_t client_address_len = 0;

    while (true) 
    {
		int echo_socket_desc = Accept(socket_desc, (struct sockaddr *)&client_address, &client_address_len);

		int n = 0;
		int maxlen = 100;
		char buffer[maxlen];

        char echo[maxlen + ECHO_STR_LEN];
        strcpy(echo, "echo: \0");
        memset(buffer, 0, maxlen);

		printf("client connected with ip address: %s\n",
		       inet_ntoa(socket_address.sin_addr));

		while ((n = recv(echo_socket_desc, buffer, maxlen, 0)) > 0)
        {
			printf("received: %s", buffer);

            strcat(echo, buffer);

			Send(echo_socket_desc, echo, strlen(echo), 0);
            memset(buffer, 0, maxlen);
            memset(echo + ECHO_STR_LEN - 1, 0, maxlen);
		}

		Close(echo_socket_desc);
	}

	Close(socket_desc);

}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}