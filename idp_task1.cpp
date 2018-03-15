#include "socket_wrapper.hpp"
#include "util.h"
#include <iostream>

static const int LISTENQ = 16;
static const int DEFAULT_ERROR = -1;
static const int SERVER_PORT = 8877;
static const char* ECHO_STR_PREFIX = "echo: ";
static const int ECHO_STR_LEN = strlen(ECHO_STR_PREFIX);

void createTestTCPServer()
{
    try 
    {
        Socket_sptr server_socket = std::make_shared<Socket>(AF_INET, SOCK_STREAM, 0, SERVER_PORT);

        server_socket->Bind(htons(SERVER_PORT), INADDR_ANY);

        server_socket->Listen(LISTENQ);

        while (true) 
        {
            Socket_sptr client_socket = server_socket->Accept();

            int n = 0;
            int maxlen = 100;
            char buffer[maxlen];

            char echo[maxlen + ECHO_STR_LEN];
            strcpy(echo, ECHO_STR_PREFIX);
            memset(buffer, 0, maxlen);

            std::cout << "client connected with ip address:" 
            << inet_ntoa(server_socket->GetSockAddrStruct()->sin_addr) 
            <<std::endl;

            while (client_socket->Recv(buffer, maxlen, 0) > 0)
            {
                std::cout << "received: "
                << buffer << std::endl;

                strcat(echo, buffer);

                client_socket->Send(echo, strlen(echo), 0);
                memset(buffer, 0, maxlen);
                memset(echo + ECHO_STR_LEN, 0, maxlen);
            }

		    client_socket->Close();
	    }

	    server_socket->Close();
    }
    catch (SocketException & e)
    {
        std::cout << e.what() << std::endl;
    }
}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}