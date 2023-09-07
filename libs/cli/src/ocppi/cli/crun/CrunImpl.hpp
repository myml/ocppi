#pragma once

#include "ocppi/cli/crun/Crun.hpp"
#include "ocppi/runtime/state/types/State.hpp"

namespace ocppi::cli::crun
{

struct Crun::impl {
        [[nodiscard]]
        auto doState(const Crun &crun, const runtime::ContainerID &id,
                     const std::vector<runtime::StateOption> &opts) const
                -> runtime::state::types::State;

        void doCreate(Crun &crun, const runtime::ContainerID &id,
                      const std::filesystem::path &pathToBundle,
                      const std::vector<runtime::CreateOption> &opts);

        void doStart(Crun &crun, const runtime::ContainerID &id,
                     const std::vector<runtime::StartOption> &opts);

        void doKill(Crun &crun, const runtime::ContainerID &id,
                    const runtime::Signal &signal,
                    const std::vector<runtime::KillOption> &opts);

        void doDelete(Crun &crun, const runtime::ContainerID &id,
                      const std::vector<runtime::DeleteOption> &opts);
};

}
