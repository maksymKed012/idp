#include "socket_wrapper.hpp"

static const int LISTENQ = 50;
static const int DEFAULT_ERROR = -1;
static const int SERVER_PORT = 8877;                                                           

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

    SocketWrapper* serverSocket = new SocketWrapper(AF_INET, SOCK_STREAM, 0, htons(8877));

    serverSocket->Bind();

    serverSocket->Listen(LISTENQ);

    struct sockaddr_in client_address;
	socklen_t client_address_len = 0;

    while (true) 
    {
		SocketWrapper* client_socket = serverSocket->Accept((struct sockaddr *)&client_address, &client_address_len);

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

			client_socket->Send(echo, strlen(echo), 0);
            memset(buffer, 0, maxlen);
            memset(echo + ECHO_STR_LEN - 1, 0, maxlen);
		}

		client_socket->Close();
        delete clent_socket;
	}

	server_socket->Close();
    delete server_socket;

}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}