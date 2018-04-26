#ifndef ASYNC_ENGINE
#define ASYNC_ENGINE

#include "socket_wrapper.hpp"
#include <vector>
#include <chrono>

class AsyncEngine;

using Socket_vector = std::vector<Socket_sptr>;
using AsyncEngine_uptr = std::unique_ptr<AsyncEngine>;
using AsyncEngine_sptr = std::shared_ptr<AsyncEngine>;

class AsyncEngine 
{
    fd_set                          m_fd_set_read;
    int                             m_max_fd;
    Socket_vector                   m_accepted_sockets;

public:

                                    AsyncEngine();
                                    ~AsyncEngine();
    Socket_vector                   Select(const std::chrono::milliseconds& timeout);
    void                            AddSocket(const Socket_sptr);
    void                            RemoveSocket(const Socket_sptr);
};



#endif