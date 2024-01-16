#pragma once

#include <exception>  // for exception_ptr
#include <filesystem> // for path
#include <memory>     // for unique_ptr, shared_ptr
#include <string>     // for string
#include <vector>     // for vector

#include "ocppi/cli/CLI.hpp"             // for CLI
#include "ocppi/runtime/ContainerID.hpp" // for ContainerID
#include "tl/expected.hpp"               // for expected

namespace ocppi
{
namespace types
{
struct ContainerListItem;
} // namespace types

namespace runtime
{
class CreateOption;
class DeleteOption;
class ExecOption;
class KillOption;
class ListOption;
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

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli
{

class CommonCLI : public CLI {
    protected:
        [[nodiscard]]
        auto logger() const -> const std::unique_ptr<spdlog::logger> &;

        CommonCLI(std::filesystem::path,
                  const std::unique_ptr<spdlog::logger> &);

    public:
        CommonCLI(const CommonCLI &) = delete;
        CommonCLI(CommonCLI &&) = delete;
        auto operator=(const CommonCLI &) -> CommonCLI & = delete;
        auto operator=(CommonCLI &&) -> CommonCLI & = delete;
        ~CommonCLI() override = default;

        [[nodiscard]]
        auto bin() const noexcept -> const std::filesystem::path & override;

        [[nodiscard]]
        auto state(const runtime::ContainerID &id) const noexcept
                -> tl::expected<runtime::state::types::State,
                                std::exception_ptr> override;
        [[nodiscard]]
        auto
        state(const runtime::ContainerID &id,
              const std::vector<std::unique_ptr<const runtime::StateOption>>
                      &opts) const noexcept
                -> tl::expected<runtime::state::types::State,
                                std::exception_ptr> override;

        [[nodiscard]]
        auto create(const runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto
        create(const runtime::ContainerID &id,
               const std::filesystem::path &pathToBundle,
               const std::vector<std::unique_ptr<const runtime::CreateOption>>
                       &opts) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto start(const runtime::ContainerID &id) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto
        start(const runtime::ContainerID &id,
              const std::vector<std::unique_ptr<const runtime::StartOption>>
                      &opts) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto kill(const runtime::ContainerID &id,
                  const runtime::Signal &signal) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto kill(const runtime::ContainerID &id, const runtime::Signal &signal,
                  const std::vector<std::unique_ptr<const runtime::KillOption>>
                          &opts) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto delete_(const runtime::ContainerID &id) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto
        delete_(const runtime::ContainerID &id,
                const std::vector<std::unique_ptr<const runtime::DeleteOption>>
                        &opts) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto exec(const runtime::ContainerID &id, const std::string &executable,
                  const std::vector<std::string> &command) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto exec(const runtime::ContainerID &id, const std::string &executable,
                  const std::vector<std::string> &command,
                  const std::vector<std::unique_ptr<const runtime::ExecOption>>
                          &opts) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto list() noexcept
                -> tl::expected<std::vector<types::ContainerListItem>,
                                std::exception_ptr> override;
        [[nodiscard]]
        auto list(const std::vector<std::unique_ptr<const runtime::ListOption>>
                          &opts) noexcept
                -> tl::expected<std::vector<types::ContainerListItem>,
                                std::exception_ptr> override;

    private:
        std::filesystem::path bin_;
        const std::unique_ptr<spdlog::logger> &logger_;
};

}
