#pragma once

#include <array>
#include <utility>
#include <functional>

template<typename... Ts>
struct TList {
    template<typename T>
    struct one { constexpr static size_t value = 1; };

    constexpr static size_t list_size = (one<Ts>::value + ...);

    template<size_t I, typename Next, typename... Other>
    struct get_helper {
        using result = typename get_helper<I - 1, Other...>::result;
    };

    template<typename Next, typename... Other>
    struct get_helper<0, Next, Other...> {
        using result = Next;
    };

    template<size_t I>
    using get = typename get_helper<I, Ts...>::result;

    template<typename Action>
    requires (list_size > 0)
    constexpr static auto map(Action&& op) {
        return std::invoke([operation = std::forward<Action>(op)]
                <size_t... Ints> (std::integer_sequence<size_t, Ints...>) {
            using ReturnType = decltype(operation(get<0>{}));

            std::array<ReturnType, list_size> results;
            ((results[Ints] = operation(get<Ints>{})), ... );

            return results;
        }, std::make_integer_sequence<size_t, list_size>{});
    }
};

// Compile-time tests
static_assert(TList<bool, int, double>::list_size == 3);
static_assert(std::is_same_v<TList<bool, int>::get<0>, bool>);
static_assert(std::is_same_v<TList<bool, int>::get<1>, int>);

//static_assert(TList<bool, int>::map([](auto arg) { return typeof(arg); })[0] == "bool");
