#pragma once

#include "ocppi/runtime/GlobalOption.hpp"

namespace ocppi::runtime
{

struct ExecOption : public GlobalOption {
        std::vector<std::string> extra;
};

}
