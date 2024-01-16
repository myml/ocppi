#pragma once

#include <string>
#include <vector>

namespace ocppi::runtime
{

class Option {
    public:
        Option() = default;
        Option(const Option &) = delete;
        Option(Option &&) = delete;
        Option &operator=(const Option &) = delete;
        Option &operator=(Option &&) = delete;
        virtual ~Option() = default;

        [[nodiscard]]
        virtual auto args() const noexcept -> std::vector<std::string> = 0;
};

}
