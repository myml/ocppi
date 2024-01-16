#include "ocppi/runtime/OutputFormatOption.hpp"

namespace ocppi::runtime
{

OutputFormatOption::OutputFormatOption(const Format &format)
        : format(format)
{
}

auto OutputFormatOption::args() const noexcept -> std::vector<std::string>
{
        std::vector<std::string> args = { "-f" };

        if (format == Format::Json) {
                args.emplace_back("json");
                return args;
        }

        args.emplace_back("text");
        return args;
}

}
