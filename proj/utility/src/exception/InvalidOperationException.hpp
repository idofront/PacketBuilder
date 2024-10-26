#ifndef UTILITY__EXCEPTION__INVALID_OPERATION_EXCEPTION_HPP__
#define UTILITY__EXCEPTION__INVALID_OPERATION_EXCEPTION_HPP__

#include <stdexcept>

namespace Utility
{
namespace Exception
{
class InvalidOperationException : public std::logic_error
{
  public:
    InvalidOperationException(const std::string &message) : std::logic_error(message)
    {
    }
};
} // namespace Exception
} // namespace Utility

#endif
