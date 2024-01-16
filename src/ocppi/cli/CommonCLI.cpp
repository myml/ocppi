#include "ocppi/cli/CommonCLI.hpp"

#include <errno.h> // for ENOENT

#include <algorithm>    // for max
#include <list>         // for list, operator!=
#include <map>          // for operator==, operator!=
#include <system_error> // for generic_category, sys...
#include <utility>      // for move

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
#include "ocppi/runtime/Signal.hpp"                 // for Signal
#include "ocppi/runtime/StartOption.hpp"            // for StartOption
#include "ocppi/runtime/StateOption.hpp"            // for StateOption
#include "ocppi/runtime/state/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/runtime/state/types/State.hpp"      // for State
#include "ocppi/types/ContainerListItem.hpp"        // for ContainerListItem
#include "ocppi/types/Generators.hpp"               // IWYU pragma: keep
#include "spdlog/spdlog.h"                          // for SPDLOG_LOGGER_DEBUG

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli
{

CommonCLI::CommonCLI(std::filesystem::path bin,
                     std::shared_ptr<spdlog::logger> logger)
        : bin_(std::move(bin))
        , logger_(std::move(logger))
{
        if (std::filesystem::exists(this->bin())) {
                return;
        }
        throw std::system_error(ENOENT, std::generic_category());
}

auto CommonCLI::bin() const noexcept -> const std::filesystem::path &
{
        return this->bin_;
}

auto CommonCLI::logger() const -> const std::shared_ptr<spdlog::logger> &
{
        return this->logger_;
}

namespace
{

auto doState(const std::string &bin,
             [[maybe_unused]] const std::shared_ptr<spdlog::logger> &logger,
             const runtime::ContainerID &id,
             const std::vector<runtime::StateOption> &opts)
        -> runtime::state::types::State
{
        std::list<std::string> args;

        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.push_front("state");
        args.push_back(id);

        SPDLOG_LOGGER_DEBUG(logger, "running {} with arguments: {}", bin, args);

        boost::process::ipstream out_ips;
        auto ret = boost::process::system(bin,
                                          boost::process::args(std::move(args)),
                                          boost::process::std_out > out_ips);
        if (ret) {
                throw CommandFailedError(ret, bin);
        }

        auto j = nlohmann::json::parse(out_ips);
        return j.get<runtime::state::types::State>();
}

}

auto CommonCLI::state(const runtime::ContainerID &id) const noexcept
        -> tl::expected<runtime::state::types::State, std::exception_ptr>
{
        return this->state(id, {});
}

auto CommonCLI::state(
        const runtime::ContainerID &id,
        const std::vector<runtime::StateOption> &opts) const noexcept
        -> tl::expected<runtime::state::types::State, std::exception_ptr>
try {
        return doState(this->bin(), this->logger(), id, opts);
} catch (...) {
        return tl::unexpected(std::current_exception());
}

namespace
{

void doCreate(const std::string &bin,
              [[maybe_unused]] const std::shared_ptr<spdlog::logger> &logger,
              const runtime::ContainerID &id,
              const std::filesystem::path &pathToBundle,
              const std::vector<runtime::CreateOption> &opts)
{
        std::list<std::string> args{
                "-b",
                pathToBundle,
        };

        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.push_front("create");
        args.push_back(id);

        SPDLOG_LOGGER_DEBUG(logger, "running {} with arguments: {}", bin, args);

        auto ret = boost::process::system(
                bin, boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, bin);
        }
}

}

auto CommonCLI::create(const runtime::ContainerID &id,
                       const std::filesystem::path &pathToBundle) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->create(id, pathToBundle, {});
}

auto CommonCLI::create(const runtime::ContainerID &id,
                       const std::filesystem::path &pathToBundle,
                       const std::vector<runtime::CreateOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doCreate(this->bin(), this->logger(), id, pathToBundle, opts);
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

namespace
{

void doStart(const std::string &bin,
             [[maybe_unused]] const std::shared_ptr<spdlog::logger> &logger,
             const runtime::ContainerID &id,
             const std::vector<runtime::StartOption> &opts)
{
        std::list<std::string> args;

        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.push_front("start");
        args.push_back(id);

        SPDLOG_LOGGER_DEBUG(logger, "running {} with arguments: {}", bin, args);

        auto ret = boost::process::system(
                bin, boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, bin);
        }
}

}

auto CommonCLI::start(const runtime::ContainerID &id) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->start(id, {});
}

auto CommonCLI::start(const runtime::ContainerID &id,
                      const std::vector<runtime::StartOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doStart(this->bin(), this->logger(), id, opts);
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

namespace
{

void doKill(const std::string &bin,
            [[maybe_unused]] const std::shared_ptr<spdlog::logger> &logger,
            const runtime::ContainerID &id, const runtime::Signal &signal,
            const std::vector<runtime::KillOption> &opts)
{
        std::list<std::string> args;

        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.push_front("kill");
        args.push_back(dynamic_cast<const std::string &>(id));
        args.push_back(dynamic_cast<const std::string &>(signal));

        SPDLOG_LOGGER_DEBUG(logger, "running {} with arguments: {}", bin, args);

        auto ret = boost::process::system(
                bin, boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, bin);
        }
}

}

auto CommonCLI::kill(const runtime::ContainerID &id,
                     const runtime::Signal &signal) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->kill(id, signal, {});
}

auto CommonCLI::kill(const runtime::ContainerID &id,
                     const runtime::Signal &signal,
                     const std::vector<runtime::KillOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doKill(this->bin(), this->logger(), id, signal, opts);
        return {};

} catch (...) {
        return tl::unexpected(std::current_exception());
}

namespace
{

void doDelete(const std::string &bin,
              [[maybe_unused]] const std::shared_ptr<spdlog::logger> &logger,
              const runtime::ContainerID &id,
              const std::vector<runtime::DeleteOption> &opts)
{
        std::list<std::string> args;
        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.push_front("delete");
        args.push_back(id);

        SPDLOG_LOGGER_DEBUG(logger, "running {} with arguments: {}", bin, args);

        auto ret = boost::process::system(
                bin, boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, bin);
        }
}

}

auto CommonCLI::delete_(const runtime::ContainerID &id) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->delete_(id, {});
}

auto CommonCLI::delete_(const runtime::ContainerID &id,
                        const std::vector<runtime::DeleteOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doDelete(this->bin(), this->logger(), id, opts);
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

namespace
{

void doExec(const std::string &bin,
            [[maybe_unused]] const std::shared_ptr<spdlog::logger> &logger,
            const runtime::ContainerID &id, const std::string &executable,
            const std::vector<std::string> &command,
            const std::vector<runtime::ExecOption> &opts)
{
        std::list<std::string> args;
        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.push_front("exec");
        args.push_back(id);
        args.push_back(executable);
        args.insert(args.end(), command.begin(), command.end());

        SPDLOG_LOGGER_DEBUG(logger, "running {} with arguments: {}", bin, args);

        auto ret = boost::process::system(
                bin, boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, bin);
        }
}

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
                     const std::vector<runtime::ExecOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doExec(this->bin(), this->logger(), id, executable, command, opts);
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

namespace
{

tl::expected<std::vector<types::ContainerListItem>, std::exception_ptr>
doList(const std::string &bin,
       [[maybe_unused]] const std::shared_ptr<spdlog::logger> &logger,
       const std::vector<runtime::ListOption> &opts)
{
        std::list<std::string> args;
        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.push_front("list");
        args.push_back("-f");
        args.push_back("json");
        SPDLOG_LOGGER_DEBUG(logger, "running {} with arguments: {}", bin, args);

        boost::process::ipstream out_ips;
        auto ret = boost::process::system(bin,
                                          boost::process::args(std::move(args)),
                                          boost::process::std_out > out_ips);
        if (ret) {
                throw CommandFailedError(ret, bin);
        }

        auto j = nlohmann::json::parse(out_ips);
        return j.get<std::vector<types::ContainerListItem>>();
}

}

auto CommonCLI::list() noexcept
        -> tl::expected<std::vector<types::ContainerListItem>,
                        std::exception_ptr>
{
        return this->list({});
}

auto CommonCLI::list(const std::vector<runtime::ListOption> &opts) noexcept
        -> tl::expected<std::vector<types::ContainerListItem>,
                        std::exception_ptr>
try {
        return doList(this->bin(), this->logger(), opts);
} catch (...) {
        return tl::unexpected(std::current_exception());
}

}
