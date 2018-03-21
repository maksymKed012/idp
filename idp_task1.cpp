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
    char buffer[BUFFER_MAXLEN];

    char echo[BUFFER_MAXLEN + ECHO_STR_LEN];
    strcpy(echo, ECHO_STR_PREFIX);
    memset(buffer, 0, BUFFER_MAXLEN);

    try 
    {
        while ((n = client_socket->Recv(buffer, BUFFER_MAXLEN, 0)))
        {
            std::cout << "received: " << buffer << std::endl;

            strcat(echo, buffer);

            client_socket->Send(echo, strlen(echo), 0);
            memset(buffer, 0, BUFFER_MAXLEN);
            memset(echo + ECHO_STR_LEN, 0, BUFFER_MAXLEN);
        }
    }
    catch (SocketException & e)
    {
        std::cout << e.what() << std::endl;
    }

    client_socket->Close();
}

void createTestTCPServer()
{
    std::vector<std::thread> listeningThreads;

    Socket_sptr server_socket = std::make_shared<Socket>(AF_INET, SOCK_STREAM, 0);

    server_socket->Bind(INADDR_ANY, SERVER_PORT);

    server_socket->Listen(LISTENQ);

    while (true) 
    {
        Socket_sptr client_socket = server_socket->Accept();

        std::cout << "client connected with ip address: " 
        << client_socket->GetIPAddressStr() 
        <<std::endl;

        listeningThreads.push_back(std::thread(listeningThread, std::move(client_socket)));
    }

    server_socket->Close();
    
}

int main(int argc, char** argv)
{
    createTestTCPServer();
    return 0;
}