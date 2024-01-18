#pragma once

#include <exception>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "ocppi/cli/CLI.hpp"
#include "ocppi/runtime/ContainerID.hpp"
#include "tl/expected.hpp"

namespace ocppi
{
namespace types
{
struct ContainerListItem;
} // namespace types

namespace runtime
{
struct CreateOption;
struct DeleteOption;
struct ExecOption;
struct KillOption;
struct ListOption;
class Signal;
struct StartOption;
struct StateOption;
struct GlobalOption;
struct RunOption;

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

class CommonCLI : public virtual CLI {
    protected:
        CommonCLI(std::filesystem::path,
                  const std::unique_ptr<spdlog::logger> &);

        [[nodiscard]]
        auto logger() const -> const std::unique_ptr<spdlog::logger> &;

        [[nodiscard]]
        virtual auto generateGlobalOptions(const runtime::GlobalOption &option)
                const noexcept -> std::vector<std::string>;

        [[nodiscard]]
        virtual auto generateSubcommandOptions(
                const runtime::CreateOption &option) const noexcept
                -> std::vector<std::string>;
        [[nodiscard]]
        virtual auto generateSubcommandOptions(
                const runtime::DeleteOption &option) const noexcept
                -> std::vector<std::string>;
        [[nodiscard]]
        virtual auto generateSubcommandOptions(
                const runtime::ExecOption &option) const noexcept
                -> std::vector<std::string>;
        [[nodiscard]]
        virtual auto generateSubcommandOptions(
                const runtime::KillOption &option) const noexcept
                -> std::vector<std::string>;
        [[nodiscard]]
        virtual auto generateSubcommandOptions(
                const runtime::ListOption &option) const noexcept
                -> std::vector<std::string>;
        [[nodiscard]]
        virtual auto generateSubcommandOptions(
                const runtime::StartOption &option) const noexcept
                -> std::vector<std::string>;
        [[nodiscard]]
        virtual auto generateSubcommandOptions(
                const runtime::StateOption &option) const noexcept
                -> std::vector<std::string>;
        [[nodiscard]]
        virtual auto generateSubcommandOptions(const runtime::RunOption &option)
                const noexcept -> std::vector<std::string>;

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
        auto state(const runtime::ContainerID &id,
                   const runtime::StateOption &option) const noexcept
                -> tl::expected<runtime::state::types::State,
                                std::exception_ptr> override;

        [[nodiscard]]
        auto create(const runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto create(const runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle,
                    const runtime::CreateOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto start(const runtime::ContainerID &id) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto start(const runtime::ContainerID &id,
                   const runtime::StartOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto kill(const runtime::ContainerID &id,
                  const runtime::Signal &signal) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto kill(const runtime::ContainerID &id, const runtime::Signal &signal,
                  const runtime::KillOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto delete_(const runtime::ContainerID &id) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto delete_(const runtime::ContainerID &id,
                     const runtime::DeleteOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto exec(const runtime::ContainerID &id, const std::string &executable,
                  const std::vector<std::string> &command) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto exec(const runtime::ContainerID &id, const std::string &executable,
                  const std::vector<std::string> &command,
                  const runtime::ExecOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto list() noexcept
                -> tl::expected<std::vector<types::ContainerListItem>,
                                std::exception_ptr> override;
        [[nodiscard]]
        auto list(const runtime::ListOption &option) noexcept
                -> tl::expected<std::vector<types::ContainerListItem>,
                                std::exception_ptr> override;

        [[nodiscard]]
        auto run(const runtime::ContainerID &id,
                 const std::filesystem::path &pathToBundle) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto run(const runtime::ContainerID &id,
                 const std::filesystem::path &pathToBundle,
                 const runtime::RunOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

    private:
        std::filesystem::path bin_;
        const std::unique_ptr<spdlog::logger> &logger_;
};

}
