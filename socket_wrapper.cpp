#include "socket_wrapper.hpp"
#include "socket_exception.hpp"
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

Socket::Socket(int handle,  sockaddr* address)
: m_socket_handle(handle)
{
    bzero(&m_socket_address, sizeof(sockaddr_in));
    sockaddr_in* temp_address = reinterpret_cast<sockaddr_in*>(address);
    m_socket_address.sin_addr = temp_address->sin_addr;
    m_socket_address.sin_family = temp_address->sin_family;
    m_socket_address.sin_port = temp_address->sin_port;
}

Socket::Socket(int domain, int type, int protocol)
{
    m_socket_handle = socket(domain, type, protocol);
    m_socket_address.sin_family = domain;

    if(m_socket_handle == -1)
    {
        throw SocketException("socket function returned error");
    }
}

Socket::~Socket()
{
    Close();
}

void Socket::SetInAddressWithStr(const char* ip_address_str)
{
    if((inet_pton(AF_INET, ip_address_str, &m_socket_address.sin_addr)) <= 0)
        throw SocketException("inet_pton returned error");
}

void Socket::InitSocketAddress(int family, int address, int port)
{
    bzero(&m_socket_address, sizeof(sockaddr_in));

    m_socket_address.sin_family = family;
    m_socket_address.sin_addr.s_addr = address;
    m_socket_address.sin_port = port;
}

void Socket::Bind(const unsigned address, const unsigned port)
{
    m_socket_address.sin_addr.s_addr = address;
    m_socket_address.sin_port = htons(port);

    if(bind(m_socket_handle, (const sockaddr*)&m_socket_address, sizeof(m_socket_address)) < 0)
        throw SocketException("bind returned error");
}

void Socket::Bind(const char* ip_address_str, const unsigned port)
{
    SetInAddressWithStr(ip_address_str);
    m_socket_address.sin_port = htons(port);

    if(bind(m_socket_handle, (const sockaddr*)&m_socket_address, sizeof(m_socket_address)) < 0)
        throw SocketException("bind returned error");
}

void Socket::Listen(int backlog)
{
    if(listen(m_socket_handle, backlog) < 0)
        throw SocketException("listen returned error");
}

Socket_sptr Socket::Accept()
{
    struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
    int client_socket_handle = accept(m_socket_handle, (sockaddr*)&client_address, &client_address_len);
    if(client_socket_handle == -1) throw SocketException("accept returned error");
    return std::make_shared<Socket>(client_socket_handle, (sockaddr*)&client_address);
}

void Socket::Close()
{
    if(close(m_socket_handle) < 0)
        throw SocketException("close returned error");
}

int Socket::Send(const void* buffer, size_t buffer_length, int flags)
{
    int retVal = send(m_socket_handle, buffer, buffer_length, flags);
    if(retVal < 0)
       throw SocketException("send returned error");

    return retVal;
}

int Socket::Recv(void* buffer, size_t buffer_length, int flags)
{
    int retVal = recv(m_socket_handle, buffer, buffer_length, flags);
    if(retVal < 0)
        throw SocketException("recv returned error");

    return retVal;
}

sockaddr_in Socket::RecvDGram(void* buffer, size_t buffer_length, int flags)
{
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);
    int retVal = recvfrom(m_socket_handle, buffer, buffer_length, flags, (sockaddr*)&client_address, &address_length);

    return retVal;
}

int Socket::Read(void* buffer, size_t buffer_length)
{
    int retVal = read(m_socket_handle, buffer, buffer_length);
    if(retVal < 0)
        throw SocketException("read returned error");

    return retVal;
}

int Socket::Write(const void* buffer, size_t buffer_length)
{
    int retVal = write(m_socket_handle, buffer, buffer_length);
    if(retVal < 0)
        throw SocketException("write returned error");

    return retVal;
}

const char* Socket::GetIPAddressStr()  const
{
    return inet_ntoa(m_socket_address.sin_addr);
}

const unsigned short Socket::GetPort() const
{
  return m_socket_address.sin_port;
}

Socket::operator int() const
{
    return m_socket_handle;
}

bool operator==(const Socket_sptr& right, const Socket_sptr& left)
{
    return right->GetSocketHandle() == left->GetSocketHandle();
}

bool operator==(const Socket_sptr& right, const int left)
{
    return right->GetSocketHandle() == left;
}
