#pragma once

#include "ocppi/runtime/state/types/State.hpp"

namespace ocppi::cli::crun
{
auto getStateFromJSON(std::istream &) -> runtime::state::types::State;
}
