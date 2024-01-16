#pragma once

#include "ocppi/InvalidArgumentError.hpp"

namespace ocppi::runtime::config
{

class IncompatibleVersionError : public common::InvalidArgumentError {
        using InvalidArgumentError::InvalidArgumentError;
        static constexpr const char *prefix = "incompatible version: ";

    public:
        explicit IncompatibleVersionError(const std::string &arg)
                : InvalidArgumentError(prefix + arg)
        {
        }
        explicit IncompatibleVersionError(const char *arg)
                : InvalidArgumentError(prefix + std::string(arg))
        {
        }
};

}
