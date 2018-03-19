#ifndef __SOCKET_WRAPPER_HPP__
#define __SOCKET_WRAPPER_HPP__
#include <memory>
#include <netinet/in.h>


class Socket;

using Socket_uptr = std::unique_ptr<Socket>;
using Socket_sptr = std::shared_ptr<Socket>;

class Socket
{
    private:
    
    int m_socket_handle;
    sockaddr_in m_socket_address;

    public:

                            Socket(int handle, sockaddr* address);
                            Socket(int domain, int type, int protocol);
    void                    SetInAddressWithStr(const char* ip_address_str);
    void                    InitSocketAddress(int family, int address, int port);
    void                    Bind(const char* ip_address_str, const unsigned port);
    void                    Bind(const unsigned address, const unsigned port);
    void                    Listen(int backlog);
    Socket_sptr             Accept();
    void                    Close();
    int                     Send(const void* buffer, size_t buffer_len, int flags);
    int                     GetSocketHandle() const {return m_socket_handle;}
    const sockaddr_in*      GetSockAddrStruct() const {return &m_socket_address;}
    int                     Recv(void* buffer, size_t buffer_length, int flags);
    int                     Read(void* buffer, size_t buffer_length);
    void                    Write(const void* buffer, size_t buffer_length);
};

#endif