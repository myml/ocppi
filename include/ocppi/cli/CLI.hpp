#pragma once
#include <filesystem>

#include "ocppi/runtime/Runtime.hpp" // for Runtime

namespace ocppi::cli
{

class CLI : public virtual runtime::Runtime {
    public:
        CLI() = default;
        CLI(const CLI &) = delete;
        CLI(CLI &&) = delete;
        auto operator=(const CLI &) -> CLI & = delete;
        auto operator=(CLI &&) -> CLI & = delete;
        virtual ~CLI() = default;

        [[nodiscard]]
        virtual auto bin() const noexcept -> const std::filesystem::path & = 0;
};

}
