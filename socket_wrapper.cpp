#include "socket_wrapper.hpp"

SocketWrapper::SocketWrapper(int handle, const sockaddr* address) 
: m_socket_handle(handle)
{
    bzero(&m_socket_address, sizeof(sockaddr_in));
    m_socket_address.sin_addr = ((const sockaddr_in*)address)->sin_addr;
    m_socket_address.sin_family = ((const sockaddr_in*)address)->sin_family;
    m_socket_address.sin_port = ((const sockaddr_in*)address)->sin_port;
}

SocketWrapper::SocketWrapper(int domain, int type, int protocol, int port)
{
    m_socket_handle = socket(domain, type, protocol);

    if(m_socket_handle != -1)
    {
        InitSocketAddress(AF_INET, htonl(INADDR_ANY), htons(port));
    }
}

bool SocketWrapper::InitSocketAddress(int family, int address, int port)
{
    bzero(&m_socket_address, sizeof(sockaddr_in));

    m_socket_address.sin_family = family;
    m_socket_address.sin_addr.s_addr = address;
    m_socket_address.sin_port = port;

    return true;
}

bool SocketWrapper::SetInAddressWithStr(const char* ip_address_str)
{
    if((inet_pton(AF_INET, ip_address_str, &m_socket_address.sin_addr)) <= 0)
        return false;

    return true;
}

bool SocketWrapper::Bind()
{
    if(bind(m_socket_handle, (const sockaddr*)&m_socket_address, sizeof(m_socket_address)) < 0)
        return false;

    return true;
}

bool SocketWrapper::Listen(int flag)
{
    if(listen(m_socket_handle, flag) < 0)
        return false;

    return true;
}

std::unique_ptr<SocketWrapper> SocketWrapper::Accept()
{
    struct sockaddr_in client_address;
	socklen_t client_address_len = 0;
    int client_socket_handle = accept(m_socket_handle, (sockaddr*)&client_address, &client_address_len);
    return std::make_unique<SocketWrapper>(client_socket_handle, (sockaddr*)&client_address);
}

bool SocketWrapper::Close()
{
    if(close(m_socket_handle) < 0)
        return false;

    return true;
}

bool SocketWrapper::Send(const void* buffer, size_t buffer_length, int flags)
{
    if(send(m_socket_handle, buffer, buffer_length, flags) < 0)
        return false;

    return true;
}


