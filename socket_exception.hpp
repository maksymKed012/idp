#include <exception>
#include <string>

/* You'll have the same (and more safe) result with just
typedef std::runtime_error socket_exception;
   or, in more modern way
using socket_exception = std::runtime_error;
   Also you can think about extending std::runtime_error with integer error
   code, it might be useful (or not :))
*/

class socket_exception : public std::exception
{
    private:

    std::string m_what_str;

    public:

    socket_exception(const char* what_str) : std::exception(), m_what_str(what_str) {}

    virtual const char* what() const noexcept override  {return m_what_str.c_str();}
};