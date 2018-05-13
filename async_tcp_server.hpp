#ifndef ASYNC_TCP_SERVER
#define ASYNC_TCP_SERVER

#include "socket_wrapper.hpp"
#include "async_engine.hpp"
#include <vector>

using Socket_vector = std::vector<Socket_sptr>;

class AsyncTCPServer
{

    AsyncEngine_uptr                m_async_engine;
    Socket_sptr                     m_master_socket;

    void                            AcceptIncomingConnection();
    void                            CloseConnection(Socket_sptr);
    bool                            ShouldConnectClient(Socket_sptr);
    void                            SendEcho(Socket_sptr, const char*, const int);
    void                            SendEchoDGram();

    public:

                                    AsyncTCPServer();
                                    ~AsyncTCPServer();
    void                            RunAsyncServer(const int server_port);
};



#endif
