#include "async_tcp_server.hpp"

static const int SERVER_PORT = 8877;

int main(int argc, char** argv)
{
    AsyncTCPServer server;
    server.RunAsyncServer(SERVER_PORT);
    
    return 0;
}
