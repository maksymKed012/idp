#include "socket_wrapper.hpp"
#include "socket_exception.hpp"
#include <iostream>
#include <string.h>
#include <vector>
#include <thread>

static const int BUFFER_MAXLEN = 100;
static const int LISTENQ = 16;
static const int SERVER_PORT = 8877;
static const char* ECHO_STR_PREFIX = "echo: ";
static const int ECHO_STR_LEN = strlen(ECHO_STR_PREFIX);

void listeningThread(Socket_sptr client_socket)
{
    int n = 0;
    int maxlen = 100;
    char buffer[maxlen];

    char echo[maxlen + ECHO_STR_LEN];
    strcpy(echo, ECHO_STR_PREFIX);
    memset(buffer, 0, maxlen);

    while ((n = client_socket->Recv(buffer, maxlen, 0)) > 0)
    {
        printf("received: %s", buffer);

        strcat(echo, buffer);

        client_socket->Send(echo, strlen(echo), 0);
        memset(buffer, 0, maxlen);
        memset(echo + ECHO_STR_LEN, 0, maxlen);
    }

    client_socket->Close();
}

void createTestTCPServer()
{
    std::vector<std::thread> listeningThreads;

    try 
    {
        Socket_sptr server_socket = std::make_shared<Socket>(AF_INET, SOCK_STREAM, 0);

        server_socket->Bind(INADDR_ANY, SERVER_PORT);

        server_socket->Listen(LISTENQ);

        while (true) 
        {
            Socket_sptr client_socket = server_socket->Accept();

            int received_data_length = 0;
            
            char buffer[BUFFER_MAXLEN];

            char echo[BUFFER_MAXLEN + ECHO_STR_LEN];
            strcpy(echo, ECHO_STR_PREFIX);
            memset(buffer, 0, BUFFER_MAXLEN);

            std::cout << "client connected with ip address: " 
            << client_socket->GetIPAddressStr() 
            <<std::endl;

            listeningThreads.push_back(std::thread(listeningThread, std::move(client_socket)));
	    }

	    server_socket->Close();
    }
    catch (SocketException & e)
    {
        printf("%s",e.what());
        for (std::thread& listeningThread : listeningThreads)
            listeningThread.join();
    }
}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}