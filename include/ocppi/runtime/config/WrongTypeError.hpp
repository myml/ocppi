#pragma once

#include "ocppi/runtime/config/InvalidConfigError.hpp"

namespace ocppi::runtime::config
{
class WrongTypeError : public InvalidConfigError {
        using InvalidConfigError::InvalidConfigError;
        static constexpr const char *prefix = "wrong type: ";

    public:
        explicit WrongTypeError(const std::string &arg)
                : InvalidConfigError(prefix + arg)
        {
        }
        explicit WrongTypeError(const char *arg)
                : InvalidConfigError(prefix + std::string(arg))
        {
        }
};

}
