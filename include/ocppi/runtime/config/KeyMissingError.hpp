#pragma once

#include "ocppi/runtime/config/InvalidConfigError.hpp"

namespace ocppi::runtime::config
{
class KeyMissingError : public InvalidConfigError {
        using InvalidConfigError::InvalidConfigError;
        static constexpr const char *prefix = "missing key: ";

    public:
        explicit KeyMissingError(const std::string &arg)
                : InvalidConfigError(prefix + arg)
        {
        }
        explicit KeyMissingError(const char *arg)
                : InvalidConfigError(prefix + std::string(arg))
        {
        }
};

}
