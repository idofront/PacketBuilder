#ifndef UTILITY__EXCEPTION__NOTIMPLEMENTEDEXCEPTION_HPP__
#define UTILITY__EXCEPTION__NOTIMPLEMENTEDEXCEPTION_HPP__

#include <stdexcept>

namespace Utility
{
namespace Exception
{
class NotImplementedException : public std::logic_error
{
  public:
    NotImplementedException(const std::string &message) : std::logic_error(message)
    {
    }
};
} // namespace Exception
} // namespace Utility

#endif
