#ifndef UTILITY__EXCEPTION__INVALID_ARGUMENT_EXCEPTION_HPP__
#define UTILITY__EXCEPTION__INVALID_ARGUMENT_EXCEPTION_HPP__

#include <stdexcept>

namespace Utility
{
namespace Exception
{
class InvalidArgumentException : public std::logic_error
{
  public:
    InvalidArgumentException(const std::string &message) : std::logic_error(message)
    {
    }
};
} // namespace Exception
} // namespace Utility

#endif
