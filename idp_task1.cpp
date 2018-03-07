#include "socket_wrapper.hpp"
#include "util.h"

static const int LISTENQ = 50;
static const int DEFAULT_ERROR = -1;
static const int SERVER_PORT = 8877;
static const int ECHO_STR_LEN = 6;                                            

void createTestTCPServer()
{
    SocketWrapper* server_socket = new SocketWrapper(PF_INET, SOCK_STREAM, 0, htons(SERVER_PORT));

    server_socket->Bind();

    server_socket->Listen(LISTENQ);

    struct sockaddr_in client_address;
	socklen_t client_address_len = 0;

    while (true) 
    {
		SocketWrapper* client_socket = server_socket->Accept((struct sockaddr *)&client_address, &client_address_len);

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
        delete client_socket;
	}

	server_socket->Close();
    delete server_socket;

}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}