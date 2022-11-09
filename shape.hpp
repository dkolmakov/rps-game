#pragma once

#include <algorithm>
#include "tlist.hpp"

template<std::size_t N>
struct StringLiteral {
    char value[N];
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
};

template<typename... Ts>
struct Beats {
    using beats = TList<Ts...>;
};

template<StringLiteral sl>
struct Label {
//    static std::string get_label() {
//        return std::string(sl.value);
//    }
    static constexpr std::string_view label = std::string_view(sl.value);
};

template<char sym>
struct Symbol {
    static constexpr char symbol = sym;
};

template<typename F>
concept Figure = requires(F)
{
    ([]<typename... Ts> (TList<Ts...>) {})(typename F::beats {});
    ([](std::string_view) {})( F::label );
    ([](const char&) {})( F::symbol );
};

