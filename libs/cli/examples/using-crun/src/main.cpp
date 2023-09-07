#include <iostream>

#include "ocppi/cli/crun/Crun.hpp"
#include "ocppi/runtime/ContainerID.hpp"
#include "ocppi/runtime/Signal.hpp"
#include "ocppi/runtime/state/types/Generators.hpp"
#include "ocppi/runtime/state/types/State.hpp"

auto main() -> int
{
        ocppi::cli::crun::Crun crun("/usr/bin/crun");
        auto state = crun.state(
                "c295070d57bf626199e95cdd80b121f69f3c0797bfc1bed5f270fb43f86ea64e");

        if (!state.has_value()) {
                auto e = state.error();

                try {
                        std::rethrow_exception(e);
                } catch (const std::exception &e) {
                        std::cerr << e.what() << std::endl;
                }

                return -1;
        }

        nlohmann::json j = state.value();
        std::cout << j.dump(1, '\t') << std::endl;

        auto result = crun.kill(
                "c295070d57bf626199e95cdd80b121f69f3c0797bfc1bed5f270fb43f86ea64e",
                "SIGTERM");

        if (!result.has_value()) {
                auto e = result.error();

                try {
                        std::rethrow_exception(e);
                } catch (const std::exception &e) {
                        std::cerr << e.what() << std::endl;
                }

                return -1;
        }
}
