#pragma once

#include "ocppi/runtime/ExecOption.hpp"
#include "ocppi/runtime/SpecRuntime.hpp"

namespace ocppi::runtime
{

class Runtime : public SpecRuntime {
    public:
        virtual auto exec(const ContainerID &id, const std::string &executable,
                          const std::vector<std::string> &command,
                          const std::vector<ExecOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
};

}
