#pragma once

#include <exception> // for exception_ptr
#include <string>    // for string
#include <vector>    // for vector

#include "ocppi/runtime/ListOption.hpp"
#include "ocppi/runtime/SpecRuntime.hpp" // for SpecRuntime
#include "ocppi/runtime/list/types/Item.hpp"
#include "tl/expected.hpp" // for expected
namespace ocppi
{
namespace runtime
{
class ContainerID;
class RunOption;
class ExecOption;
} // namespace runtime
} // namespace ocppi

namespace ocppi::runtime
{

class Runtime : public SpecRuntime {
    public:
        virtual auto
        run(const runtime::ContainerID &id,
            const std::filesystem::path &pathToBundle,
            const std::vector<runtime::RunOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
        virtual auto exec(const ContainerID &id, const std::string &executable,
                          const std::vector<std::string> &command,
                          const std::vector<ExecOption> &opts = {}) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
        virtual auto list(const std::vector<ListOption> &opts = {}) noexcept
                -> tl::expected<std::list<list::types::Item>,
                                std::exception_ptr> = 0;
};

}
