
#include "util.h"
#include <memory>

class SocketWrapper
{
    private:
    
    int m_socket_handle;
    sockaddr_in m_socket_address;

    private:

    SocketWrapper(const SocketWrapper&) = delete;
    SocketWrapper& operator=(const SocketWrapper&) = delete;

    public:

                              SocketWrapper(int handle, const sockaddr* address);
                              SocketWrapper(int domain, int type, int protocol, int port);
    void                      SetInAddressWithStr(const char* ip_address_str);
    void                      InitSocketAddress(int family, int address, int port);
    void                      Bind();
    void                      Listen(int flag);
    // just as suggestion - you can use typedefs with some suffixes for smart pointers.
    // like unique_ptr = _uptr, shared_ptr = _sptr.
    // so std::unique_ptr<SocketWrapper> = SocketWrapper_uptr
    std::unique_ptr<SocketWrapper> Accept();
    void                      Close();
    void                      Send(const void* buffer, size_t buffer_len, int flags);
    int                       GetSocketHandle() const {return m_socket_handle;}
    const sockaddr_in*        GetSockAddrStruct() const {return &m_socket_address;}

};