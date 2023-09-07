#pragma once

#include <experimental/propagate_const>

#include "ocppi/runtime/Runtime.hpp"
#include "spdlog/logger.h"

namespace ocppi::cli
{

class CLI : public runtime::Runtime {
    protected:
        [[nodiscard]]
        auto bin() const -> const std::filesystem::path &;
        [[nodiscard]]
        auto logger() const -> const std::shared_ptr<spdlog::logger> &;

        CLI(std::filesystem::path, std::shared_ptr<spdlog::logger>);

    public:
        CLI(const CLI &) = delete;
        CLI(CLI &&) = delete;
        auto operator=(const CLI &) -> CLI & = delete;
        auto operator=(CLI &&) -> CLI & = delete;
        virtual ~CLI() = default;

    private:
        std::filesystem::path bin_;
        std::shared_ptr<spdlog::logger> logger_;
};

}
