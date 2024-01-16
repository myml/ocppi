#include <unistd.h> // for isatty

#include <algorithm>   // for max
#include <cstdio>      // for stderr
#include <exception>   // for exception_ptr, exception
#include <iostream>    // for basic_ostream, endl
#include <map>         // for operator!=, operator==
#include <memory>      // for shared_ptr, unique_ptr
#include <mutex>       // for mutex
#include <string>      // for char_traits, operator<<
#include <string_view> // for string_view
#include <utility>     // for move
#include <vector>      // for vector

#include <bits/types/struct_FILE.h> // for _IO_FILE

#include "nlohmann/json.hpp"                        // for basic_json
#include "nlohmann/json_fwd.hpp"                    // for json
#include "ocppi/cli/CLI.hpp"                        // for CLI
#include "ocppi/cli/crun/Crun.hpp"                  // for Crun
#include "ocppi/runtime/Signal.hpp"                 // for Signal
#include "ocppi/runtime/state/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/runtime/state/types/State.hpp"      // for State
#include "ocppi/types/ContainerListItem.hpp"        // for ContainerListItem
#include "ocppi/types/Generators.hpp"               // IWYU pragma: keep
#include "spdlog/common.h"                   // for color_mode, level_enum
#include "spdlog/logger.h"                   // for logger
#include "spdlog/sinks/ansicolor_sink.h"     // for ansicolor_stderr_sink
#include "spdlog/sinks/stdout_color_sinks.h" // for stderr_color_sink_mt
#include "spdlog/sinks/systemd_sink.h"       // for systemd_sink_mt, syst...
#include "spdlog/spdlog.h"                   // for SPDLOG_LOGGER_ERROR
#include "tl/expected.hpp"                   // for expected

namespace spdlog
{
namespace sinks
{
class sink;
} // namespace sinks
} // namespace spdlog

void printException(const std::unique_ptr<spdlog::logger> &logger,
                    std::string_view msg, std::exception_ptr ptr) noexcept
try {
        std::rethrow_exception(ptr);
} catch (const std::exception &e) {
        SPDLOG_LOGGER_ERROR(logger, "{}: {}", msg, e.what());
} catch (...) {
        SPDLOG_LOGGER_ERROR(logger, "{}: unknown exception", msg);
}

auto main() -> int
{
        std::unique_ptr<spdlog::logger> logger;
        {
                auto sinks = std::vector<std::shared_ptr<spdlog::sinks::sink>>(
                        { std::make_shared<spdlog::sinks::systemd_sink_mt>(
                                "ocppi") });
                if (isatty(stderr->_fileno)) {
                        sinks.push_back(std::make_shared<
                                        spdlog::sinks::stderr_color_sink_mt>());
                }

                logger = std::make_unique<spdlog::logger>(
                        "ocppi", sinks.begin(), sinks.end());

                logger->set_level(spdlog::level::trace);
        }

        try {
                std::unique_ptr<ocppi::cli::CLI> cli;
                {
                        auto crun = ocppi::cli::crun::Crun::New("/usr/bin/crun",
                                                                logger);
                        if (!crun.has_value()) {
                                printException(logger, "New crun object failed",
                                               crun.error());
                                return -1;
                        }
                        cli = std::move(crun.value());
                }

                auto bin = cli->bin();
                SPDLOG_LOGGER_DEBUG(logger,
                                    R"(Using OCI runtime CLI program "{}")",
                                    bin.string());

                auto list = cli->list();
                if (!list.has_value()) {
                        printException(logger, "Run crun list failed",
                                       list.error());
                        return -1;
                }

                if (list->empty()) {
                        SPDLOG_LOGGER_ERROR(logger, "No container exists.");
                        return 0;
                }

                for (auto item : list.value()) {
                        nlohmann::json j = item;
                        SPDLOG_LOGGER_INFO(logger, "Existing container {}",
                                           j.dump());
                }

                auto state = cli->state(list->front().id);

                if (!state.has_value()) {
                        printException(logger, "Run crun state failed",
                                       state.error());
                        return -1;
                }

                nlohmann::json j = state.value();
                std::cout << j.dump(1, '\t') << std::endl;

                auto killResult = cli->kill(list->front().id,
                                            ocppi::runtime::Signal("SIGTERM"));

                if (!killResult.has_value()) {
                        printException(logger, "Run crun kill failed",
                                       killResult.error());
                        return -1;
                }

                return 0;
        } catch (...) {
                printException(logger, "Failed to kill first crun container",
                               std::current_exception());
                return -1;
        }

        return 0;
}
