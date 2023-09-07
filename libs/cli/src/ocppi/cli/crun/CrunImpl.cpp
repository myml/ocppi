#include "ocppi/cli/crun/CrunImpl.hpp"

#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>

#include <boost/process/args.hpp>
#include <boost/process/io.hpp>
#include <boost/process/pipe.hpp>
#include <boost/process/system.hpp>

#include "ocppi/cli/CLIImpl.hpp"
#include "ocppi/cli/CommandFailedError.hpp"
#include "ocppi/cli/crun/deserialize.hpp"

namespace ocppi::cli::crun
{

auto Crun::impl::doState(const Crun &crun, const runtime::ContainerID &id,
                         const std::vector<runtime::StateOption> &opts) const
        -> runtime::state::types::State
{
        std::list<std::string> args;

        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.emplace_back("state");
        args.push_back(reinterpret_cast<const std::string &>(id));

        boost::process::ipstream out_ips;
        auto ret = boost::process::system(crun.CLI::pImpl->bin.string(),
                                          boost::process::args(std::move(args)),
                                          boost::process::std_out > out_ips);
        if (ret) {
                throw CommandFailedError(ret, crun.CLI::pImpl->bin.string());
        }

        return getStateFromJSON(out_ips);
}

void Crun::impl::doCreate(Crun &crun, const runtime::ContainerID &id,
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

        args.emplace_back("create");
        args.push_back(reinterpret_cast<const std::string &>(id));

        auto ret =
                boost::process::system(crun.CLI::pImpl->bin.string(),
                                       boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, crun.CLI::pImpl->bin.string());
        }
}

void Crun::impl::doStart(Crun &crun, const runtime::ContainerID &id,
                         const std::vector<runtime::StartOption> &opts)
{
        std::list<std::string> args;

        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.emplace_back("start");
        args.push_back(reinterpret_cast<const std::string &>(id));

        auto ret =
                boost::process::system(crun.CLI::pImpl->bin.string(),
                                       boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, crun.CLI::pImpl->bin.string());
        }
}

void Crun::impl::doKill(Crun &crun, const runtime::ContainerID &id,
                        const runtime::Signal &signal,
                        const std::vector<runtime::KillOption> &opts)
{
        std::list<std::string> args;

        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.emplace_back("kill");
        args.push_back(reinterpret_cast<const std::string &>(id));
        args.push_back(reinterpret_cast<const std::string &>(signal));

        auto ret =
                boost::process::system(crun.CLI::pImpl->bin.string(),
                                       boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, crun.CLI::pImpl->bin.string());
        }
}

void Crun::impl::doDelete(Crun &crun, const runtime::ContainerID &id,
                          const std::vector<runtime::DeleteOption> &opts)
{
        std::list<std::string> args;
        for (auto &opt : opts) {
                args.splice(args.end(), opt.args());
        }

        args.emplace_back("delete");
        args.push_back(reinterpret_cast<const std::string &>(id));

        auto ret =
                boost::process::system(crun.CLI::pImpl->bin.string(),
                                       boost::process::args(std::move(args)));
        if (ret) {
                throw CommandFailedError(ret, crun.CLI::pImpl->bin.string());
        }
}

}
