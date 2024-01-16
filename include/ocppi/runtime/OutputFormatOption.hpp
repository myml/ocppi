#pragma once

#include <string> // for string
#include <vector> // for vector

#include "ocppi/runtime/GlobalOption.hpp" // for GlobalOption

namespace ocppi::runtime
{
class OutputFormatOption : public GlobalOption {
    public:
        enum class Format { Text, Json };
        explicit OutputFormatOption(const Format &format);
        auto args() const noexcept -> std::vector<std::string> override;

    private:
        Format format;
};
}
