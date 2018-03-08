#include "socket_wrapper.hpp"
#include "socket_exception.hpp"

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
    else throw socket_exception("socket function returned error");
}

void SocketWrapper::InitSocketAddress(int family, int address, int port)
{
    bzero(&m_socket_address, sizeof(sockaddr_in));

    m_socket_address.sin_family = family;
    m_socket_address.sin_addr.s_addr = address;
    m_socket_address.sin_port = port;
}

void SocketWrapper::SetInAddressWithStr(const char* ip_address_str)
{
    if((inet_pton(AF_INET, ip_address_str, &m_socket_address.sin_addr)) <= 0)
        throw socket_exception("inet_pton returned error");
}

void SocketWrapper::Bind()
{
    if(bind(m_socket_handle, (const sockaddr*)&m_socket_address, sizeof(m_socket_address)) < 0)
        throw socket_exception("bind returned error");
}

void SocketWrapper::Listen(int flag)
{
    if(listen(m_socket_handle, flag) < 0)
        throw socket_exception("listen returned error");
}

std::unique_ptr<SocketWrapper> SocketWrapper::Accept()
{
    struct sockaddr_in client_address;
	socklen_t client_address_len = 0;
    int client_socket_handle = accept(m_socket_handle, (sockaddr*)&client_address, &client_address_len);
    if(client_socket_handle == -1) throw socket_exception("accept returned error");
    return std::make_unique<SocketWrapper>(client_socket_handle, (sockaddr*)&client_address);
}

void SocketWrapper::Close()
{
    if(close(m_socket_handle) < 0)
        throw socket_exception("close returned error");
}

void SocketWrapper::Send(const void* buffer, size_t buffer_length, int flags)
{
    if(send(m_socket_handle, buffer, buffer_length, flags) < 0)
       throw socket_exception("send returned error");
}


