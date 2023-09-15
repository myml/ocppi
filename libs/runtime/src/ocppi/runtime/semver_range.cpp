#include "ocppi/runtime/semver_range.hpp"

#include "ocppi/runtime/configure.hpp" // for oci_runtime_spec_max, oci_runtime_spe...
#include "semver.hpp"                  // for from_string, version (ptr only)

namespace ocppi::runtime
{

auto support_spec_semver_range()
        -> const std::pair<semver::version, semver::version> &
{
        static const auto &min_ = semver::from_string(oci_runtime_spec_min);
        static const auto &max_ = semver::from_string(oci_runtime_spec_max);
        static const auto &range_ = std::make_pair(min_, max_);
        return range_;
}

}
