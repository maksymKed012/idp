#include "async_engine.hpp"
#include "socket_exception.hpp"
#include <iostream>
#include <sys/select.h>

AsyncEngine::AsyncEngine()
{
    FD_ZERO(&m_fd_set_read);
}

AsyncEngine::~AsyncEngine()
{
}

Socket_vector AsyncEngine::Select(const std::chrono::milliseconds & timeout)
{
    fd_set temp_set;
    memcpy(&temp_set, &m_fd_set_read, sizeof(m_fd_set_read));
    size_t nfds = m_max_fd + 1;
    timeval dt;
    std::chrono::seconds full_seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    std::chrono::microseconds useconds = timeout - full_seconds;
    dt.tv_sec = full_seconds.count();
    dt.tv_usec = useconds.count();
    int ready_sockets_count = select(nfds, &temp_set,  NULL, NULL, &dt);
    if(ready_sockets_count < 0)
        throw SocketException("select() failed");

    Socket_vector ready_sockets;

    for (auto& socket : m_accepted_sockets)
    {
        if(FD_ISSET(*socket, &temp_set))
        {
            ready_sockets.push_back(socket);
        }
    }

    return ready_sockets;
}

void AsyncEngine::AddSocket(Socket_sptr socket) 
{
    if(socket.get())
    {
        FD_SET(*socket, &m_fd_set_read);
        m_accepted_sockets.push_back(socket);
        if(*socket > m_max_fd)
            m_max_fd = *socket; 
    }
}

void AsyncEngine::RemoveSocket(Socket_sptr socket)
{
    FD_CLR(*socket, &m_fd_set_read);
    auto it = std::find(m_accepted_sockets.begin(), m_accepted_sockets.end(), socket);
    if(it != m_accepted_sockets.end())
        m_accepted_sockets.erase(it);

    if(*socket == m_max_fd)
    {
        m_max_fd = 0;

        for(auto& socket : m_accepted_sockets)
        {
            if(socket->GetSocketHandle() > m_max_fd)
                m_max_fd = *socket;
        }
    }
}
