#include "ocppi/cli/crun/Crun.hpp"

#include "spdlog/spdlog.h" // IWYU pragma: keep

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli::crun
{

auto Crun::New(const std::filesystem::path &bin,
               const std::unique_ptr<spdlog::logger> &logger) noexcept
        -> tl::expected<std::unique_ptr<Crun>, std::exception_ptr>
try {
        return std::unique_ptr<Crun>(new Crun(bin, logger));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

}
