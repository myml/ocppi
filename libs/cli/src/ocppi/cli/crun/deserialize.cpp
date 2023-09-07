#include "ocppi/cli/crun/deserialize.hpp"

#include "ocppi/runtime/state/types/Generators.hpp"

namespace ocppi::cli::crun
{
auto getStateFromJSON(std::istream &is) -> runtime::state::types::State
{
        auto j = nlohmann::json::parse(is);
        return j.get<runtime::state::types::State>();
}
}
