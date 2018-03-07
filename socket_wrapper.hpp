
#include "util.h"

class SocketWrapper
{
    private:
    
    int m_socket_handle;
    sockaddr_in m_socket_address;


    public:

                              SocketWrapper(int handle, const sockaddr* address);
                              SocketWrapper(int domain, int type, int protocol, int port);
    bool                      SetInAddressWithStr(const char* ip_address_str);
    bool                      InitSocketAddress(int family, int address, int port);
    bool                      Bind();
    bool                      Listen(int flag);
    SocketWrapper*            Accept(sockaddr* client_sockaddr, socklen_t* socket_length);
    bool                      Close();
    bool                      Send(const void* buffer, size_t buffer_len, int flags);
    int                       GetSocketHandle() const {return m_socket_handle;}
    const sockaddr_in*        GetSockAddrStruct() const {return &m_socket_address;}

};