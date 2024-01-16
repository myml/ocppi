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
class GlobalOption : public CreateOption,
                     public DeleteOption,
                     public ExecOption,
                     public KillOption,
                     public ListOption,
                     public StartOption,
                     public StateOption {
    public:
        GlobalOption() = default;
        GlobalOption(const GlobalOption &) = delete;
        GlobalOption(GlobalOption &&) = delete;
        GlobalOption &operator=(const GlobalOption &) = delete;
        GlobalOption &operator=(GlobalOption &&) = delete;
        ~GlobalOption() override = default;
};
}
