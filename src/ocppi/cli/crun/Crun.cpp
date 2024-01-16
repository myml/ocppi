#include "ocppi/cli/crun/Crun.hpp"

#include "ocppi/cli/format.hpp"                     // IWYU pragma: keep
#include "ocppi/runtime/state/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/types/Generators.hpp"               // IWYU pragma: keep

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli::crun
{

auto Crun::New(const std::filesystem::path &bin,
               const std::shared_ptr<spdlog::logger> &logger) noexcept
        -> tl::expected<std::unique_ptr<Crun>, std::exception_ptr>
try {
        return std::unique_ptr<Crun>(new Crun(bin, logger));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

}
