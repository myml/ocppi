#pragma once

#include <exception>  // for exception_ptr
#include <filesystem> // for path
#include <memory>     // for make_shared, shared_ptr, unique_ptr

#include "ocppi/cli/CommonCLI.hpp" // for CommonCLI
#include "tl/expected.hpp"         // for expected

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
