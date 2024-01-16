#pragma once

#include "ocppi/runtime/CreateOption.hpp"
#include "ocppi/runtime/DeleteOption.hpp"
#include "ocppi/runtime/ExecOption.hpp"
#include "ocppi/runtime/KillOption.hpp"
#include "ocppi/runtime/ListOption.hpp"
#include "ocppi/runtime/StartOption.hpp"
#include "ocppi/runtime/StateOption.hpp"
namespace ocppi::runtime
{
class GlobalOption : public virtual CreateOption,
                     public virtual DeleteOption,
                     public virtual ExecOption,
                     public virtual KillOption,
                     public virtual ListOption,
                     public virtual StartOption,
                     public virtual StateOption {
    public:
        GlobalOption() = default;
        GlobalOption(const GlobalOption &) = delete;
        GlobalOption(GlobalOption &&) = delete;
        GlobalOption &operator=(const GlobalOption &) = delete;
        GlobalOption &operator=(GlobalOption &&) = delete;
        ~GlobalOption() override = default;
};
}
