#pragma once

#include <exception>
#include <filesystem>
#include <memory>

#include "ocppi/cli/CommonCLI.hpp"
#include "tl/expected.hpp"

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli::crun
{

class Crun final : public CommonCLI {
        using CommonCLI::CommonCLI;

    public:
        static auto New(const std::filesystem::path &bin,
                        const std::unique_ptr<spdlog::logger> &logger) noexcept
                -> tl::expected<std::unique_ptr<Crun>, std::exception_ptr>;
};
}
