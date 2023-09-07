#include "ocppi/cli/CLI.hpp"

#include "ocppi/cli/CLIImpl.hpp"

namespace ocppi::cli
{

CLI::CLI(const std::filesystem::path &bin) noexcept
        : pImpl(new impl{ bin })
{
}

CLI::~CLI() = default;

}
