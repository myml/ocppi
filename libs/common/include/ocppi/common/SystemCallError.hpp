#pragma once

#include <stdexcept>
#include <system_error>

namespace ocppi::common
{

class SystemCallError : public std::system_error {
        using std::system_error::system_error;

        static constexpr const char *prefix = "system call failed: ";

    public:
        explicit SystemCallError(const int ret, const std::string &arg)
                : std::system_error(
                          std::error_code(errno, std::system_category()),
                          prefix + arg + "[retval: " + std::to_string(ret) +
                                  "]")
        {
        }

        explicit SystemCallError(const int ret, const char *arg)
                : SystemCallError(ret, std::string(arg))
        {
        }
};

}
