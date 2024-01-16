#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "ocppi/runtime/ContainerID.hpp"
#include "ocppi/runtime/CreateOption.hpp"
#include "ocppi/runtime/DeleteOption.hpp"
#include "ocppi/runtime/KillOption.hpp"
#include "ocppi/runtime/StartOption.hpp"
#include "ocppi/runtime/StateOption.hpp"
#include "ocppi/runtime/state/types/State.hpp"
#include "tl/expected.hpp"

namespace ocppi::runtime
{

namespace state::types
{
struct State;
}

class Signal;

class SpecRuntime {
    public:
        SpecRuntime() = default;
        SpecRuntime(const SpecRuntime &) = delete;
        SpecRuntime(SpecRuntime &&) = delete;
        auto operator=(const SpecRuntime &) -> SpecRuntime & = delete;
        auto operator=(SpecRuntime &&) -> SpecRuntime & = delete;

        [[nodiscard]]
        virtual auto state(const ContainerID &id) const noexcept
                -> tl::expected<state::types::State, std::exception_ptr> = 0;
        [[nodiscard]]
        virtual auto state(const ContainerID &id,
                           const std::vector<StateOption> &opts) const noexcept
                -> tl::expected<state::types::State, std::exception_ptr> = 0;

        [[nodiscard]]
        virtual auto create(const ContainerID &id,
                            const std::filesystem::path &pathToBundle) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
        [[nodiscard]]
        virtual auto create(const ContainerID &id,
                            const std::filesystem::path &pathToBundle,
                            const std::vector<CreateOption> &opts) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;

        [[nodiscard]]
        virtual auto start(const ContainerID &id) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
        [[nodiscard]]
        virtual auto start(const ContainerID &id,
                           const std::vector<StartOption> &opts) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;

        [[nodiscard]]
        virtual auto kill(const ContainerID &id, const Signal &signal) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
        [[nodiscard]]
        virtual auto kill(const ContainerID &id, const Signal &signal,
                          const std::vector<KillOption> &opts) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;

        [[nodiscard]]
        virtual auto delete_(const ContainerID &id) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
        [[nodiscard]]
        virtual auto delete_(const ContainerID &id,
                             const std::vector<DeleteOption> &opts) noexcept
                -> tl::expected<void, std::exception_ptr> = 0;
};

}
