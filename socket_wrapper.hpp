
#include "util.h"
#include <memory>

class SocketWrapper;

using SocketWrapper_uptr = std::unique_ptr<SocketWrapper>;
using SocketWrapper_sptr = std::shared_ptr<SocketWrapper>;

class SocketWrapper
{
    private:
    
    int m_socket_handle;
    sockaddr_in m_socket_address;

    private:

    SocketWrapper(const SocketWrapper&) = delete;
    SocketWrapper& operator=(const SocketWrapper&) = delete;

    public:

                              SocketWrapper(int handle, sockaddr* address);
                              SocketWrapper(int domain, int type, int protocol, int port);
    void                      SetInAddressWithStr(const char* ip_address_str);
    void                      InitSocketAddress(int family, int address, int port);
    void                      Bind(const sockaddr_in* socket_address);
    void                      Listen(int flag);
    SocketWrapper_uptr        Accept();
    void                      Close();
    void                      Send(const void* buffer, size_t buffer_len, int flags);
    int                       GetSocketHandle() const {return m_socket_handle;}
    const sockaddr_in*        GetSockAddrStruct() const {return &m_socket_address;}
    int                       ReceiveData(void* buffer, size_t buffer_length, int flags);
    int                       ReadDataToBuffer(void* buffer, size_t buffer_length);
    void                      WriteDataFromBuffer(const void* buffer, size_t buffer_length);
};