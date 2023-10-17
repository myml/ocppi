#include "ocppi/cli/CLI.hpp"

#include <errno.h> // for ENOENT

#include <system_error> // for generic_category, system_error
#include <utility>      // for move

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli
{

CLI::CLI(std::filesystem::path bin, std::shared_ptr<spdlog::logger> logger)
        : bin_(std::move(bin))
        , logger_(std::move(logger))
{
        if (std::filesystem::exists(this->bin())) {
                return;
        }
        throw std::system_error(ENOENT, std::generic_category());
}

auto CLI::bin() const -> const std::filesystem::path &
{
        return this->bin_;
}

auto CLI::logger() const -> const std::shared_ptr<spdlog::logger> &
{
        return this->logger_;
}

}
