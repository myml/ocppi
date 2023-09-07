#pragma once

#include "ocppi/cli/CLI.hpp"

namespace ocppi::cli::crun
{

class Crun final : public CLI {
        using CLI::CLI;

    public:
        static auto New(const std::filesystem::path &bin,
                        const std::shared_ptr<spdlog::logger> &logger =
                                std::make_shared<spdlog::logger>("")) noexcept
                -> tl::expected<std::unique_ptr<Crun>, std::exception_ptr>;

        [[nodiscard]]
        auto
        state(const runtime::ContainerID &id,
              const std::vector<runtime::StateOption> &opts = {}) const noexcept
                -> tl::expected<runtime::state::types::State,
                                std::exception_ptr> override;

        [[nodiscard]]
        auto
        create(const runtime::ContainerID &id,
               const std::filesystem::path &pathToBundle,
               const std::vector<runtime::CreateOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto start(const runtime::ContainerID &id,
                   const std::vector<runtime::StartOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto kill(const runtime::ContainerID &id, const runtime::Signal &signal,
                  const std::vector<runtime::KillOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto
        delete_(const runtime::ContainerID &id,
                const std::vector<runtime::DeleteOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto exec(const runtime::ContainerID &id, const std::string &executable,
                  const std::vector<std::string> &command,
                  const std::vector<runtime::ExecOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> override;
};

}
