#pragma once

#include "ocppi/cli/CLI.hpp"

namespace ocppi::cli
{

struct CLI::impl {
        std::filesystem::path bin;
};

}
