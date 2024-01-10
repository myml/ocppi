#pragma once

#include <exception>  // for exception_ptr
#include <filesystem> // for path
#include <memory>     // for make_shared, shared_ptr, unique_ptr
#include <string>     // for string
#include <vector>     // for vector

#include "ocppi/cli/CLI.hpp" // for CLI
#include "spdlog/logger.h"
#include "tl/expected.hpp" // for expected

namespace ocppi
{
namespace runtime
{
class ContainerID;
class CreateOption;
class DeleteOption;
class ExecOption;
class KillOption;
class Signal;
class StartOption;
class StateOption;
class ListOption;
namespace state
{
namespace types
{
struct State;
} // namespace types
} // namespace state
namespace list
{
namespace types
{
struct Item;
}; // namespace types
} // namespace list

} // namespace runtime
} // namespace ocppi

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

        [[nodiscard]]
        auto list(const std::vector<runtime::ListOption> &opts = {}) noexcept
                -> tl::expected<std::list<runtime::list::types::Item>,
                                std::exception_ptr> override;
};
}
