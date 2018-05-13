#ifndef SOCKET_WRAPPER_HPP
#define SOCKET_WRAPPER_HPP
#include <memory>
#include <netinet/in.h>


class Socket;

using Socket_uptr = std::unique_ptr<Socket>;
using Socket_sptr = std::shared_ptr<Socket>;

bool             operator==(const Socket_sptr&, const Socket_sptr&);
bool             operator==(const Socket_sptr&, const int);

class Socket
{
    private:

    int                     m_socket_handle;
    sockaddr_in             m_socket_address;

    public:

                            Socket(int handle, sockaddr* address);
                            Socket(int domain, int type, int protocol);
                            ~Socket();
    void                    SetInAddressWithStr(const char* ip_address_str);
    void                    InitSocketAddress(int family, int address, int port);
    void                    Bind(const char* ip_address_str, const unsigned port);
    void                    Bind(const unsigned address, const unsigned port);
    void                    Listen(int backlog);
    Socket_sptr             Accept();
    void                    Close();
    int                     Send(const void* buffer, size_t buffer_len, int flags);
    int                     Recv(void* buffer, size_t buffer_length, int flags);
    sockaddr_in             RecvDGram(void* buffer, size_t buffer_length, int flags);
    int                     Read(void* buffer, size_t buffer_length);
    int                     Write(const void* buffer, size_t buffer_length);
    int                     GetSocketHandle() const {return m_socket_handle;}
    const sockaddr_in*      GetSockAddrStruct() const {return &m_socket_address;}
    const char*             GetIPAddressStr() const;
    const unsigned short    GetPort() const;

                            operator int() const;
};

#endif
