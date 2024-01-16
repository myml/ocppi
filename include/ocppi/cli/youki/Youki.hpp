#pragma once

#include <exception>  // for exception_ptr
#include <filesystem> // for path
#include <memory>     // for unique_ptr
#include <vector>     // for vector

#include <experimental/propagate_const> // for propagate_const

#include "ocppi/cli/CLI.hpp" // for CLI
#include "tl/expected.hpp"   // for expected

namespace ocppi
{
namespace runtime
{
class ContainerID;
class CreateOption;
class DeleteOption;
class KillOption;
class Signal;
class StartOption;
class StateOption;
namespace state
{
namespace types
{
struct State;
} // namespace types
} // namespace state
} // namespace runtime
} // namespace ocppi

namespace ocppi::cli::youki
{

class Youki : public CLI {
    public:
        using CLI::CLI;
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

    protected:
        struct impl;

        friend struct impl;
        std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
};

}
