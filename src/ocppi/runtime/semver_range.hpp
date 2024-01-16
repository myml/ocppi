#pragma once

#include <utility> // for pair

namespace semver
{
struct version;
} // namespace semver

namespace ocppi::runtime
{

auto support_spec_semver_range()
        -> const std::pair<semver::version, semver::version> &;

}
