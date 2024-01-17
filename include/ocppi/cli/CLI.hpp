#pragma once
#include <filesystem>

#include "ocppi/runtime/Runtime.hpp"

namespace ocppi::cli
{

class CLI : public virtual runtime::Runtime {
    public:
        [[nodiscard]]
        virtual auto bin() const noexcept -> const std::filesystem::path & = 0;
};

}
