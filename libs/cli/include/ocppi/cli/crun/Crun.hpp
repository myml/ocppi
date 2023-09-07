#pragma once

#include "ocppi/cli/CLI.hpp"

namespace ocppi::cli::crun
{

class Crun : public CLI {
    protected:
        struct impl;
        friend struct impl;
        std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;

    public:
        Crun(const std::filesystem::path &bin);
        [[nodiscard]]
        auto
        state(const runtime::ContainerID &id,
              const std::vector<runtime::StateOption> &opts = {}) const noexcept
                -> tl::expected<runtime::state::types::State,
                                std::exception_ptr> override;

        auto
        create(const runtime::ContainerID &id,
               const std::filesystem::path &pathToBundle,
               const std::vector<runtime::CreateOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        auto start(const runtime::ContainerID &id,
                   const std::vector<runtime::StartOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        auto kill(const runtime::ContainerID &id, const runtime::Signal &signal,
                  const std::vector<runtime::KillOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        auto
        delete_(const runtime::ContainerID &id,
                const std::vector<runtime::DeleteOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        ~Crun() override;
};

}
