#include <exception>
#include <errno.h>
#include <string>

class SocketException : public std::runtime_error
{
    int m_error;

    public:

    SocketException(const char* error_str) : std::runtime_error(error_str), m_error(errno) {}

    virtual const char* what() const noexcept override
    {
        std::string ret_str(std::runtime_error::what());
        ret_str.append(" ").append(strerror(m_error));
        return ret_str.c_str();
    }

    const int get_errno() const {return m_error;} 
};