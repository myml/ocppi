#include "ocppi/cli/CommonCLI.hpp"

#include <algorithm>
#include <cerrno>
#include <istream>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include "boost/process/args.hpp"
#include "boost/process/io.hpp"
#include "boost/process/pipe.hpp"
#include "boost/process/system.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "ocppi/cli/CommandFailedError.hpp"
#include "ocppi/runtime/ContainerID.hpp"
#include "ocppi/runtime/CreateOption.hpp"
#include "ocppi/runtime/DeleteOption.hpp"
#include "ocppi/runtime/ExecOption.hpp"
#include "ocppi/runtime/GlobalOption.hpp"
#include "ocppi/runtime/KillOption.hpp"
#include "ocppi/runtime/ListOption.hpp"
#include "ocppi/runtime/RunOption.hpp"
#include "ocppi/runtime/Signal.hpp"
#include "ocppi/runtime/StartOption.hpp"
#include "ocppi/runtime/StateOption.hpp"
#include "ocppi/runtime/state/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/runtime/state/types/State.hpp"
#include "ocppi/types/ContainerListItem.hpp"
#include "ocppi/types/Generators.hpp" // IWYU pragma: keep
#include "spdlog/fmt/ranges.h"        // IWYU pragma: keep
#include "spdlog/spdlog.h"

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli
{

namespace
{

template <typename Result>
auto doCommand(const std::string &bin,
               [[maybe_unused]] const std::unique_ptr<spdlog::logger> &logger,
               std::vector<std::string> &&globalOption,
               const std::string &command, std::vector<std::string> &&options,
               std::vector<std::string> &&arguments) -> Result
{
        arguments.insert(arguments.begin(), command);
        arguments.insert(arguments.begin(),
                         std::make_move_iterator(globalOption.begin()),
                         std::make_move_iterator(globalOption.end()));
        arguments.insert(arguments.end(),
                         std::make_move_iterator(options.begin()),
                         std::make_move_iterator(options.end()));

        SPDLOG_LOGGER_DEBUG(logger, R"(Executing "{}" with arguments: {})", bin,
                            arguments);

        if constexpr (std::is_void_v<Result>) {
                auto ret = boost::process::system(
                        bin, boost::process::args(std::move(arguments)));
                if (ret != 0) {
                        throw CommandFailedError(ret, bin);
                }
                return;
        } else {
                boost::process::ipstream out_ips;
                auto ret = boost::process::system(
                        bin, boost::process::args(std::move(arguments)),
                        boost::process::std_out > out_ips);
                if (ret != 0) {
                        throw CommandFailedError(ret, bin);
                }

                auto json_result = nlohmann::json::parse(out_ips);
                return json_result.get<Result>();
        }
}

}

CommonCLI::CommonCLI(std::filesystem::path bin,
                     const std::unique_ptr<spdlog::logger> &logger)
        : bin_(std::move(bin))
        , logger_(std::move(logger))
{
        if (std::filesystem::exists(bin_)) {
                return;
        }
        throw std::system_error(ENOENT, std::generic_category());
}

auto CommonCLI::bin() const noexcept -> const std::filesystem::path &
{
        return this->bin_;
}

auto CommonCLI::logger() const -> const std::unique_ptr<spdlog::logger> &
{
        return this->logger_;
}

auto CommonCLI::state(const runtime::ContainerID &id) const noexcept
        -> tl::expected<runtime::state::types::State, std::exception_ptr>
{
        return this->state(id, {});
}

auto CommonCLI::state(const runtime::ContainerID &id,
                      const runtime::StateOption &option) const noexcept
        -> tl::expected<runtime::state::types::State, std::exception_ptr>
try {
        return doCommand<runtime::state::types::State>(
                this->bin(), this->logger(),
                this->generateGlobalOptions(option), "state",
                this->generateSubcommandOptions(option), { id });
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::create(const runtime::ContainerID &id,
                       const std::filesystem::path &pathToBundle) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->create(id, pathToBundle, {});
}

auto CommonCLI::create(const runtime::ContainerID &id,
                       const std::filesystem::path &pathToBundle,
                       const runtime::CreateOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doCommand<void>(this->bin(), this->logger(),
                        this->generateGlobalOptions(option), "create",
                        this->generateSubcommandOptions(option),
                        { id, "-b", pathToBundle });
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::start(const runtime::ContainerID &id) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->start(id, {});
}

auto CommonCLI::start(const runtime::ContainerID &id,
                      const runtime::StartOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doCommand<void>(this->bin(), this->logger(),
                        this->generateGlobalOptions(option), "start",
                        this->generateSubcommandOptions(option), { id });
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::kill(const runtime::ContainerID &id,
                     const runtime::Signal &signal) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->kill(id, signal, {});
}

auto CommonCLI::kill(const runtime::ContainerID &id,
                     const runtime::Signal &signal,
                     const runtime::KillOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doCommand<void>(this->bin(), this->logger(),
                        this->generateGlobalOptions(option), "kill",
                        this->generateSubcommandOptions(option),
                        { id, signal });
        return {};

} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::delete_(const runtime::ContainerID &id) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->delete_(id, {});
}

auto CommonCLI::delete_(const runtime::ContainerID &id,
                        const runtime::DeleteOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doCommand<void>(this->bin(), this->logger(),
                        this->generateGlobalOptions(option), "delete",
                        this->generateSubcommandOptions(option), { id });
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::exec(const runtime::ContainerID &id,
                     const std::string &executable,
                     const std::vector<std::string> &command) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->exec(id, executable, command, {});
}

auto CommonCLI::exec(const runtime::ContainerID &id,
                     const std::string &executable,
                     const std::vector<std::string> &command,
                     const runtime::ExecOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        std::vector<std::string> arguments;
        arguments.push_back(id);
        arguments.push_back(executable);
        arguments.insert(arguments.end(), command.begin(), command.end());

        doCommand<void>(this->bin(), this->logger(),
                        this->generateGlobalOptions(option), "exec",
                        this->generateSubcommandOptions(option),
                        std::move(arguments));
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::list() noexcept
        -> tl::expected<std::vector<types::ContainerListItem>,
                        std::exception_ptr>
{
        return this->list({});
}

auto CommonCLI::list(const runtime::ListOption &option) noexcept
        -> tl::expected<std::vector<types::ContainerListItem>,
                        std::exception_ptr>
try {
        runtime::ListOption new_option = option;
        new_option.format = runtime::ListOption::OutputFormat::Json;

        return doCommand<std::vector<types::ContainerListItem>>(
                this->bin(), this->logger(),
                this->generateGlobalOptions(option), "list",
                this->generateSubcommandOptions(option), {});
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::run(const runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->run(id, pathToBundle, {});
}

[[nodiscard]]
auto CommonCLI::run(const runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle,
                    const runtime::RunOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doCommand<void>(this->bin(), this->logger(),
                        this->generateGlobalOptions(option), "run",
                        this->generateSubcommandOptions(option),
                        { id, pathToBundle.string() });
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::generateGlobalOptions(const runtime::GlobalOption &option)
        const noexcept -> std::vector<std::string>
{
        auto ret = option.extra;
        if (option.root) {
                ret.emplace_back("--root");
                ret.push_back(option.root->string());
        }
        return ret;
}

auto CommonCLI::generateSubcommandOptions(const runtime::CreateOption &option)
        const noexcept -> std::vector<std::string>
{
        return option.extra;
}

auto CommonCLI::generateSubcommandOptions(const runtime::DeleteOption &option)
        const noexcept -> std::vector<std::string>
{
        return option.extra;
}

auto CommonCLI::generateSubcommandOptions(const runtime::ExecOption &option)
        const noexcept -> std::vector<std::string>
{
        std::vector<std::string> ret = option.extra;
        if (option.tty) {
                ret.emplace_back("--tty");
        }
        if (option.cwd) {
                ret.emplace_back("--cwd");
                ret.push_back(option.cwd->string());
        }
        for (const auto &env : option.env) {
                ret.emplace_back("--env");
                ret.push_back(env.first + "=" + env.second);
        }
        if (option.uid) {
                ret.emplace_back("--user");
                ret.push_back(
                        std::to_string(*option.uid) +
                        (option.gid ? ":" + std::to_string(*option.gid) : ""));
        }
        return ret;
}

auto CommonCLI::generateSubcommandOptions(const runtime::KillOption &option)
        const noexcept -> std::vector<std::string>
{
        return option.extra;
}

auto CommonCLI::generateSubcommandOptions(const runtime::ListOption &option)
        const noexcept -> std::vector<std::string>
{
        std::vector<std::string> ret = option.extra;

        if (option.format == runtime::ListOption::OutputFormat::Json) {
                ret.emplace_back("-f");
                ret.emplace_back("json");
        }

        return ret;
}

auto CommonCLI::generateSubcommandOptions(const runtime::StartOption &option)
        const noexcept -> std::vector<std::string>
{
        return option.extra;
}

auto CommonCLI::generateSubcommandOptions(const runtime::StateOption &option)
        const noexcept -> std::vector<std::string>
{
        return option.extra;
}

auto CommonCLI::generateSubcommandOptions(const runtime::RunOption &option)
        const noexcept -> std::vector<std::string>
{
        return option.extra;
}

}
