#pragma once

#include <exception> // for exception_ptr
#include <memory>    // for unique_ptr
#include <string>    // for string
#include <vector>    // for vector

#include "ocppi/runtime/ContainerID.hpp"     // for ContainerID
#include "ocppi/runtime/SpecRuntime.hpp"     // for SpecRuntime
#include "ocppi/types/ContainerListItem.hpp" // for ContainerListItem
#include "tl/expected.hpp"                   // for expected

namespace ocppi
{
namespace runtime
{
class ExecOption;
class ListOption;
} // namespace runtime
} // namespace ocppi

namespace ocppi::runtime
{

class Runtime : public SpecRuntime {
    public:
        [[nodiscard]]
        virtual auto exec(const ContainerID &id, const std::string &executable,
                          const std::vector<std::string> &command) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
        [[nodiscard]]
        virtual auto
        exec(const ContainerID &id, const std::string &executable,
             const std::vector<std::string> &command,
             const std::vector<std::unique_ptr<const ExecOption>> &opts) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;

        [[nodiscard]]
        virtual auto list() noexcept
                -> tl::expected<std::vector<types::ContainerListItem>,
                                std::exception_ptr> = 0;
        [[nodiscard]]
        virtual auto
        list(const std::vector<std::unique_ptr<const ListOption>> &opts) noexcept
                -> tl::expected<std::vector<types::ContainerListItem>,
                                std::exception_ptr> = 0;
};

}
