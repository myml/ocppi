#include "ocppi/cli/crun/Crun.hpp"

#include <algorithm>        // for max
#include <initializer_list> // for initializer_list
#include <list>             // for list, operator!=
#include <map>              // for operator!=, operator==
#include <type_traits>      // for remove_reference<>::type
#include <utility>          // for move

#include "boost/process/args.hpp"           // for args, args_
#include "boost/process/io.hpp"             // for std_out, std_out_
#include "boost/process/pipe.hpp"           // for ipstream
#include "boost/process/system.hpp"         // for system
#include "nlohmann/json.hpp"                // for basic_json, basic_jso...
#include "nlohmann/json_fwd.hpp"            // for json
#include "ocppi/cli/CommandFailedError.hpp" // for CommandFailedError
#include "ocppi/cli/format.hpp"             // IWYU pragma: keep
#include "ocppi/runtime/ContainerID.hpp"    // for ContainerID
#include "ocppi/runtime/CreateOption.hpp"   // for CreateOption
#include "ocppi/runtime/DeleteOption.hpp"   // for DeleteOption
#include "ocppi/runtime/ExecOption.hpp"     // for ExecOption
#include "ocppi/runtime/KillOption.hpp"     // for KillOption
#include "ocppi/runtime/Signal.hpp"         // for Signal
#include "ocppi/runtime/StartOption.hpp"    // for StartOption
#include "ocppi/runtime/StateOption.hpp"    // for StateOption
#include "ocppi/runtime/state/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/runtime/state/types/State.hpp"      // for State
#include "spdlog/spdlog.h"                          // for SPDLOG_LOGGER_DEBUG

namespace spdlog
{
class logger;
} // namespace spdlog

namespace ocppi::cli::crun
{

auto Crun::New(const std::filesystem::path &bin,
               const std::shared_ptr<spdlog::logger> &logger) noexcept
        -> tl::expected<std::unique_ptr<Crun>, std::exception_ptr>
try {
        return std::unique_ptr<Crun>(new Crun(bin, logger));
} catch (...) {
        return tl::unexpected(std::current_exception());
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

auto Crun::state(const runtime::ContainerID &id,
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

auto Crun::create(const runtime::ContainerID &id,
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

auto Crun::start(const runtime::ContainerID &id,
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

auto Crun::kill(const runtime::ContainerID &id, const runtime::Signal &signal,
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

auto Crun::delete_(const runtime::ContainerID &id,
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

auto Crun::exec(const runtime::ContainerID &id, const std::string &executable,
                const std::vector<std::string> &command,
                const std::vector<runtime::ExecOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        doExec(this->bin(), this->logger(), id, executable, command, opts);
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

}
