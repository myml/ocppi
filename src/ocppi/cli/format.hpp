#pragma once

#include <vector>

#include <fmt/format.h>

template <typename Element>
class fmt::formatter<std::vector<Element>> {
    public:
        constexpr auto parse(format_parse_context &ctx)
        {
                return ctx.begin();
        }
        template <typename Context>
        constexpr auto format(std::vector<Element> const &list,
                              Context &ctx) const
        {
                format_to(ctx.out(), "[ ");
                for (const auto &e : list) {
                        format_to(ctx.out(), "{} ", e);
                }
                return format_to(ctx.out(), "]");
        }
};
