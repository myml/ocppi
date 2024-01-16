#include "ocppi/cli/CommonCLI.hpp"

#include <algorithm>    // for max
#include <cerrno>       // for ENOENT
#include <iterator>     // for make_move_iterator
#include <map>          // for operator!=, operator==
#include <string>       // for basic_string, string
#include <system_error> // for generic_category
#include <utility>      // for move
#include <vector>       // for vector

#include "boost/process/args.hpp"                   // for args, args_
#include "boost/process/io.hpp"                     // for std_out, std_out_
#include "boost/process/pipe.hpp"                   // for ipstream
#include "boost/process/system.hpp"                 // for system
#include "nlohmann/json.hpp"                        // for basic_json
#include "nlohmann/json_fwd.hpp"                    // for json
#include "ocppi/cli/CommandFailedError.hpp"         // for CommandFailedError
#include "ocppi/cli/format.hpp"                     // IWYU pragma: keep
#include "ocppi/runtime/ContainerID.hpp"            // for ContainerID
#include "ocppi/runtime/CreateOption.hpp"           // for CreateOption
#include "ocppi/runtime/DeleteOption.hpp"           // for DeleteOption
#include "ocppi/runtime/ExecOption.hpp"             // for ExecOption
#include "ocppi/runtime/KillOption.hpp"             // for KillOption
#include "ocppi/runtime/ListOption.hpp"             // for ListOption
#include "ocppi/runtime/OutputFormatOption.hpp"     // for OutputFormatOption
#include "ocppi/runtime/Signal.hpp"                 // for Signal
#include "ocppi/runtime/StartOption.hpp"            // for StartOption
#include "ocppi/runtime/StateOption.hpp"            // for StateOption
#include "ocppi/runtime/state/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/runtime/state/types/State.hpp"      // for State
#include "ocppi/types/ContainerListItem.hpp"        // for ContainerListItem
#include "ocppi/types/Generators.hpp"               // IWYU pragma: keep
#include "spdlog/spdlog.h"                          // for SPDLOG_LOGGER_DEBUG

namespace ocppi
{
namespace runtime
{
class GlobalOption;
} // namespace runtime
} // namespace ocppi

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli
{

namespace
{

template <typename Result, typename Opt>
auto doCommand(const std::string &bin,
               [[maybe_unused]] const std::unique_ptr<spdlog::logger> &logger,
               const std::string &command,
               const std::vector<std::unique_ptr<const Opt>> &opts,
               std::vector<std::string> &&arguments) -> Result
{
        static_assert(std::is_base_of_v<Opt, runtime::GlobalOption>);

        arguments.insert(arguments.begin(), command);

        for (const auto &opt : opts) {
                auto args = opt->args();
                auto loc = arguments.begin();

                if (dynamic_cast<const runtime::GlobalOption *>(opt.get()) !=
                    nullptr) {
                        loc = arguments.end();
                }

                arguments.insert(loc, std::make_move_iterator(args.begin()),
                                 std::make_move_iterator(args.end()));
        }

        SPDLOG_LOGGER_DEBUG(logger, "running {} with arguments: {}", bin,
                            arguments);

        boost::process::ipstream out_ips;
        auto ret = boost::process::system(
                bin, boost::process::args(std::move(arguments)),
                boost::process::std_out > out_ips);
        if (ret != 0) {
                throw CommandFailedError(ret, bin);
        }

        if constexpr (std::is_void_v<Result>) {
                return;
        } else {
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

auto CommonCLI::state(
        const runtime::ContainerID &id,
        const std::vector<std::unique_ptr<const runtime::StateOption>> &opts)
        const noexcept
        -> tl::expected<runtime::state::types::State, std::exception_ptr>
try {
        return doCommand<runtime::state::types::State>(
                this->bin(), this->logger(), "state", opts, { id });
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::create(const runtime::ContainerID &id,
                       const std::filesystem::path &pathToBundle) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->create(id, pathToBundle, {});
}

auto CommonCLI::create(
        const runtime::ContainerID &id,
        const std::filesystem::path &pathToBundle,
        const std::vector<std::unique_ptr<const runtime::CreateOption>>
                &opts) noexcept -> tl::expected<void, std::exception_ptr>
try {
        std::vector<std::string> arguments;
        arguments.push_back(id);
        arguments.emplace_back("-b");
        arguments.push_back(pathToBundle);

        doCommand<void>(this->bin(), this->logger(), "create", opts,
                        std::move(arguments));
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::start(const runtime::ContainerID &id) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->start(id, {});
}

auto CommonCLI::start(
        const runtime::ContainerID &id,
        const std::vector<std::unique_ptr<const runtime::StartOption>>
                &opts) noexcept -> tl::expected<void, std::exception_ptr>
try {
        doCommand<void>(this->bin(), this->logger(), "start", opts, { id });
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

auto CommonCLI::kill(
        const runtime::ContainerID &id, const runtime::Signal &signal,
        const std::vector<std::unique_ptr<const runtime::KillOption>>
                &opts) noexcept -> tl::expected<void, std::exception_ptr>
try {
        std::vector<std::string> arguments;
        arguments.push_back(id);
        arguments.push_back(signal);

        doCommand<void>(this->bin(), this->logger(), "kill", opts,
                        std::move(arguments));
        return {};

} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::delete_(const runtime::ContainerID &id) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->delete_(id, {});
}

auto CommonCLI::delete_(
        const runtime::ContainerID &id,
        const std::vector<std::unique_ptr<const runtime::DeleteOption>>
                &opts) noexcept -> tl::expected<void, std::exception_ptr>
try {
        doCommand<void>(this->bin(), this->logger(), "delete", opts, { id });
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

auto CommonCLI::exec(
        const runtime::ContainerID &id, const std::string &executable,
        const std::vector<std::string> &command,
        const std::vector<std::unique_ptr<const runtime::ExecOption>>
                &opts) noexcept -> tl::expected<void, std::exception_ptr>
try {
        std::vector<std::string> arguments;
        arguments.push_back(id);
        arguments.push_back(executable);
        arguments.insert(arguments.end(), command.begin(), command.end());

        doCommand<void>(this->bin(), this->logger(), "exec", opts,
                        std::move(arguments));
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto CommonCLI::list() noexcept
        -> tl::expected<std::vector<types::ContainerListItem>,
                        std::exception_ptr>
{
        auto format = std::make_unique<runtime::OutputFormatOption>(
                runtime::OutputFormatOption::Format::Json);
        std::vector<std::unique_ptr<const runtime::ListOption>> opt;
        opt.push_back(std::move(format));

        return this->list(opt);
}

auto CommonCLI::list(
        const std::vector<std::unique_ptr<const runtime::ListOption>>
                &opts) noexcept
        -> tl::expected<std::vector<types::ContainerListItem>,
                        std::exception_ptr>
try {
        return doCommand<std::vector<types::ContainerListItem>>(
                this->bin(), this->logger(), "list", opts, {});
} catch (...) {
        return tl::unexpected(std::current_exception());
}

}
