#include <exception>
#include <errno.h>
#include <string>

class SocketException : public std::runtime_error
{
    int m_error;
    std::string m_error_string;

    public:

    SocketException(const char* error_str) : std::runtime_error(error_str), m_error(errno) {}

    virtual const char* what() const noexcept override
    {
        m_error_string = std::runtime_error::what();
        return m_error_string.append(" ").append(strerror(m_error)).c_str();
    }

    const int get_errno() const {return m_error;} 
};