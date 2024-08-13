#include <Utility.hpp>

namespace Utility
{
std::string Demangle(const std::string &demangle)
{
    int status;
    auto demangledName = abi::__cxa_demangle(demangle.c_str(), 0, 0, &status);
    if (status != 0)
    {
        throw std::runtime_error("Failed to demangle name");
    }
    auto demangledNameAsString = std::string(demangledName);
    delete demangledName;
    return demangledNameAsString;
}
} // namespace Utility
