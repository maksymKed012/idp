#include "socket_wrapper.hpp"
#include "util.h"

static const int LISTENQ = 16;
static const int DEFAULT_ERROR = -1;
static const int SERVER_PORT = 8877;
static const char* echo_str_prefix = "echo: ";
static const int ECHO_STR_LEN = strlen(echo_str_prefix);

void createTestTCPServer()
{
    try 
    {
        SocketWrapper_uptr server_socket = std::make_unique<SocketWrapper>(AF_INET, SOCK_STREAM, 0, SERVER_PORT);

        sockaddr_in server_socket_address;
        bzero(&server_socket_address, sizeof(sockaddr_in));
        server_socket_address.sin_family = AF_INET;
        server_socket_address.sin_port = htons (SERVER_PORT);
        server_socket_address.sin_addr.s_addr = INADDR_ANY ; 

        server_socket->Bind(&server_socket_address);

        server_socket->Listen(LISTENQ);

        while (true) 
        {
            SocketWrapper_uptr client_socket = server_socket->Accept();

            int n = 0;
            int maxlen = 100;
            char buffer[maxlen];

            char echo[maxlen + ECHO_STR_LEN];
            strcpy(echo, echo_str_prefix);
            memset(buffer, 0, maxlen);

            printf("client connected with ip address: %s\n",
		       inet_ntoa(server_socket->GetSockAddrStruct()->sin_addr));

            while (client_socket->ReceiveData(buffer, maxlen, 0) > 0)
            {
                printf("received: %s", buffer);

                strcat(echo, buffer);

                client_socket->Send(echo, strlen(echo), 0);
                memset(buffer, 0, maxlen);
                memset(echo + ECHO_STR_LEN, 0, maxlen);
            }

		    client_socket->Close();
	    }

	    server_socket->Close();
    }
    catch (std::exception & e)
    {
        printf("%s",e.what());
    }
}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}