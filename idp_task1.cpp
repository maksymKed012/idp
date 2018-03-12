#include "socket_wrapper.hpp"
#include "util.h"

static const int LISTENQ = 16;
static const int DEFAULT_ERROR = -1;
static const int SERVER_PORT = 8877;
// multiple spaces in the end of this line looks strange :)
static const int ECHO_STR_LEN = 6;                                            

void createTestTCPServer()
{
    try 
    {
    std::unique_ptr<SocketWrapper> server_socket = std::make_unique<SocketWrapper>(AF_INET, SOCK_STREAM, 0, SERVER_PORT);

    server_socket->Bind();

    server_socket->Listen(LISTENQ);

// please choose one alignments/indentations style (spaces/tabs) and use it consistently
    while (true) 
    {
		std::unique_ptr<SocketWrapper> client_socket = server_socket->Accept();

		int n = 0;
		int maxlen = 100;
		char buffer[maxlen];

        // ECHO_STR_LEN is still a magic number.
        // you can for now create global variable ECHO_STR and calculate its len like
        // ECHO_STR_LEN = ECHO_STR.length()
        // hardcoded number here is a bad way because you may forgot to change it
        // when changing string itself.
        char echo[maxlen + ECHO_STR_LEN];
        strcpy(echo, "echo: \0");
        memset(buffer, 0, maxlen);

		printf("client connected with ip address: %s\n",
		       inet_ntoa(server_socket->GetSockAddrStruct()->sin_addr));

        // 'recv' also should be a part of SocketWrapper class I guess.
        // such as 'read' & 'write' (you don't need 'em now, but 'for future use' ;))
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
    catch (std::exception & e)
    {
        printf(e.what());
    }
}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}