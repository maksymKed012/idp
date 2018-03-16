#include "socket_wrapper.hpp"
#include "socket_exception.hpp"

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

    if(m_socket_handle == -1)
    {
        throw SocketException("socket function returned error");
    }
}

void Socket::InitSocketAddress(int family, int address, int port)
{
    bzero(&m_socket_address, sizeof(sockaddr_in));

    m_socket_address.sin_family = family;
    m_socket_address.sin_addr.s_addr = address;
    m_socket_address.sin_port = port;
}

void Socket::SetInAddressWithStr(const char* ip_address_str)
{
    if((inet_pton(AF_INET, ip_address_str, &m_socket_address.sin_addr)) <= 0)
        throw SocketException("inet_pton returned error");
}

void Socket::Bind(const unsigned address, const unsigned port)
{
    m_socket_address.sin_addr.s_addr = address;
    m_socket_address.sin_port = port;

    if(bind(m_socket_handle, (const sockaddr*)&m_socket_address, sizeof(m_socket_address)) < 0)
        throw SocketException("bind returned error");
}

void Socket::Listen(int flag)
{
    if(listen(m_socket_handle, flag) < 0)
        throw SocketException("listen returned error");
}

Socket_sptr Socket::Accept()
{
    struct sockaddr_in client_address;
	socklen_t client_address_len = 0;
    int client_socket_handle = accept(m_socket_handle, (sockaddr*)&client_address, &client_address_len);
    if(client_socket_handle == -1) throw SocketException("accept returned error");
    return std::make_shared<Socket>(client_socket_handle, (sockaddr*)&client_address);
}

void Socket::Close()
{
    if(close(m_socket_handle) < 0)
        throw SocketException("close returned error");
}

void Socket::Send(const void* buffer, size_t buffer_length, int flags)
{
    if(send(m_socket_handle, buffer, buffer_length, flags) < 0)
       throw SocketException("send returned error");
}

int Socket::Recv(void* buffer, size_t buffer_length, int flags)
{
    return recv(m_socket_handle, buffer, buffer_length, flags);
}

int Socket::Read(void* buffer, size_t buffer_length)
{
    return read(m_socket_handle, buffer, buffer_length);
}

void Socket::Write(const void* buffer, size_t buffer_length)
{
    if(write(m_socket_handle, buffer, buffer_length) == -1)
        throw SocketException("write returned error");
}
