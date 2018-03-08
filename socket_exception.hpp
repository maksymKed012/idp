#include <exception>
#include <string>

class socket_exception : public std::exception
{
    private:

    std::string m_what_str;

    public:

    socket_exception(const char* what_str) : std::exception(), m_what_str(what_str) {}

    virtual const char* what() const noexcept override  {return m_what_str.c_str();}
};