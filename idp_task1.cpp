#include "socket_wrapper.hpp"
#include "util.h"

static const int LISTENQ = 16;
static const int DEFAULT_ERROR = -1;
static const int SERVER_PORT = 8877;
static const int ECHO_STR_LEN = 6;                                            

void createTestTCPServer()
{
    std::unique_ptr<SocketWrapper> server_socket = std::make_unique<SocketWrapper>(AF_INET, SOCK_STREAM, 0, SERVER_PORT);

    server_socket->Bind();

    server_socket->Listen(LISTENQ);

    while (true) 
    {
		std::unique_ptr<SocketWrapper> client_socket = server_socket->Accept();

		int n = 0;
		int maxlen = 100;
		char buffer[maxlen];

        char echo[maxlen + ECHO_STR_LEN];
        strcpy(echo, "echo: \0");
        memset(buffer, 0, maxlen);

		printf("client connected with ip address: %s\n",
		       inet_ntoa(server_socket->GetSockAddrStruct()->sin_addr));

		while ((n = recv(client_socket->GetSocketHandle(), buffer, maxlen, 0)) > 0)
        {
			printf("received: %s", buffer);

            strcat(echo, buffer);

			client_socket->Send(echo, strlen(echo), 0);
            memset(buffer, 0, maxlen);
            memset(echo + ECHO_STR_LEN - 1, 0, maxlen);
		}

		client_socket->Close();
	}

	server_socket->Close();
}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}