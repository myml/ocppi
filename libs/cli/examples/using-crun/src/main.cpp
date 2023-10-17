#include <stdio.h>  // for FILE, stderr
#include <unistd.h> // for isatty

#include <exception>   // for exception, rethrow_exce...
#include <iostream>    // for operator<<, char_traits
#include <memory>      // for shared_ptr, make_shared
#include <string>      // for operator<<
#include <string_view> // for operator<<, string_view
#include <utility>     // for move
#include <vector>      // for vector

#include <bits/types/struct_FILE.h> // for _IO_FILE

#include "nlohmann/json.hpp"                        // for basic_json
#include "nlohmann/json_fwd.hpp"                    // for json
#include "ocppi/cli/CLI.hpp"                        // for CLI
#include "ocppi/cli/crun/Crun.hpp"                  // for Crun
#include "ocppi/runtime/ContainerID.hpp"            // IWYU pragma: keep
#include "ocppi/runtime/Signal.hpp"                 // IWYU pragma: keep
#include "ocppi/runtime/state/types/Generators.hpp" // IWYU pragma: keep
#include "spdlog/common.h"                          // for trace
#include "spdlog/logger.h"                          // for logger
#include "spdlog/sinks/stdout_color_sinks.h"        // for stderr_color_sink_mt
#include "spdlog/sinks/systemd_sink.h"              // for systemd_sink_mt
#include "tl/expected.hpp"                          // for expected

namespace spdlog
{
namespace sinks
{
class sink;
} // namespace sinks
} // namespace spdlog

void printException(std::string_view msg, std::exception_ptr ptr) noexcept
try {
        std::rethrow_exception(ptr);
} catch (const std::exception &e) {
        std::cerr << msg << ": " << e.what() << std::endl;
} catch (...) {
        std::cerr << msg << ": unknown exception" << std::endl;
}

auto main() -> int
{
        std::shared_ptr<spdlog::logger> logger;

        {
                auto sinks = std::vector<std::shared_ptr<spdlog::sinks::sink>>(
                        { std::make_shared<spdlog::sinks::systemd_sink_mt>(
                                "ocppi") });
                if (isatty(stderr->_fileno)) {
                        sinks.push_back(std::make_shared<
                                        spdlog::sinks::stderr_color_sink_mt>());
                }

                logger = std::make_shared<spdlog::logger>(
                        "ocppi", sinks.begin(), sinks.end());

                logger->set_level(spdlog::level::trace);
        }

        std::unique_ptr<ocppi::cli::CLI> cli;
        {
                auto crun =
                        ocppi::cli::crun::Crun::New("/usr/bin/crun", logger);
                if (!crun.has_value()) {
                        printException("new crun", crun.error());
                        return -1;
                }
                cli = std::move(crun.value());
        }

        auto state = cli->state(
                "c295070d57bf626199e95cdd80b121f69f3c0797bfc1bed5f270fb43f86ea64e");

        if (!state.has_value()) {
                printException("crun state", state.error());
                return -1;
        }

        nlohmann::json j = state.value();
        std::cout << j.dump(1, '\t') << std::endl;

        auto killResult = cli->kill(
                "c295070d57bf626199e95cdd80b121f69f3c0797bfc1bed5f270fb43f86ea64e",
                "SIGTERM");

        if (!killResult.has_value()) {
                printException("crun kill", killResult.error());
                return -1;
        }

        return 0;
}
