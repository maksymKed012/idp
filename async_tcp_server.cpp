#include "async_tcp_server.hpp"
#include "socket_exception.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sys/select.h>

static const int BUFFER_MAXLEN = 100;
static const int LISTENQ = 16;
static const int SERVER_PORT = 8877;
static const char* ECHO_STR_PREFIX = "echo: ";
static const int ECHO_STR_LEN = strlen(ECHO_STR_PREFIX);

using namespace std::chrono_literals;

void AsyncTCPServer::AcceptIncomingConnection()
{
    Socket_sptr accepted_socket = m_master_socket->Accept();
    if(ShouldConnectClient(accepted_socket))
    {
        m_async_engine->AddSocket(accepted_socket);
        std::cout << "client connected with ip address: "
        << accepted_socket->GetIPAddressStr() << ":" << accepted_socket->GetPort() << std::endl;
    }
    else 
    {
        std::cout << "client with address " << accepted_socket->GetIPAddressStr() << ":" << 
        accepted_socket->GetPort() <<  " was dropped" << std::endl;
    }
}

void AsyncTCPServer::CloseConnection(Socket_sptr socket)
{
    m_async_engine->RemoveSocket(socket);
    std::cout << "disconnected " << socket->GetIPAddressStr() << 
                        ":" << socket->GetPort() << std::endl;
}

bool AsyncTCPServer::ShouldConnectClient(Socket_sptr socket)
{
    //accepting every client connection for now..
    return true;
}

void AsyncTCPServer::SendEcho(Socket_sptr active_socket, const char* buffer, const int buffer_size)
{
    std::cout << "received " << buffer << "from " <<
    active_socket->GetIPAddressStr() << ":" << active_socket->GetPort() << std::endl;
    std::string echo_str("echo: ");
    echo_str.append(buffer);
    active_socket->Send(echo_str.c_str(), echo_str.length(), 0);
    memset((void*)buffer, 0, buffer_size);
}

AsyncTCPServer::AsyncTCPServer() 
{
    m_master_socket = std::make_shared<Socket>(AF_INET, SOCK_DGRAM, 0);
    m_async_engine = std::make_unique<AsyncEngine>();
    m_async_engine->AddSocket(m_master_socket);
}


AsyncTCPServer::~AsyncTCPServer() 
{

}

void AsyncTCPServer::RunAsyncServer(const int server_port)
{
    try 
    {
        char buffer[BUFFER_MAXLEN] = {0};
        m_master_socket->Bind(INADDR_ANY, server_port);
        m_master_socket->Listen(LISTENQ);

        while(true)
        {
            Socket_vector ready_sockets = m_async_engine->Select(1300ms);
            
            for(auto& socket : ready_sockets)
            {
                sockaddr_in client_address = socket->RecvDGram(buffer, sizeof(buffer), 0);

                SendEcho(socket, buffer, BUFFER_MAXLEN);
            }
        }

    }
    catch (SocketException & se)
    {
        std::cout << se.what() << std::endl;
    }
    catch (std::runtime_error & re)
    {
        std::cout << re.what() << std::endl;
    }
}

