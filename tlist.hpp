#pragma once

#include <vector>
#include <utility>

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

    template<typename T>
    static constexpr bool contains_v = [](){
        return (std::is_same_v<Ts, T> || ...);
    }();

    template<typename Action>
    static void for_each(Action&& op) {
        [operation = std::forward<Action>(op)]
        <size_t... Ints> (std::integer_sequence<size_t, Ints...>) {
            (operation(Ints, get<Ints>{}), ... );
        } (std::make_integer_sequence<size_t, list_size>{});
    }

    template<typename Action>
    static auto map(Action&& op) {
        [operation = std::forward<Action>(op)]
        <size_t... Ints> (std::integer_sequence<size_t, Ints...>) {
            std::vector<decltype(operation(0, get<0>{}))> results;
            (results.push_back(operation(Ints, get<Ints>{})), ... );
            return results;
        } (std::make_integer_sequence<size_t, list_size>{});
    }

    template<typename Action, typename Combination, typename Result>
    static auto map_reduce(Action&& op, Combination&& comb, Result initial) {
        return [operation = std::forward<Action>(op),
                combination = std::forward<Combination>(comb),
                initial] <size_t... Ints> (std::integer_sequence<size_t, Ints...>) {
            Result result = initial;
            ((result = combination(result, operation(Ints, get<Ints>{}))), ... );
            return result;
        } (std::make_integer_sequence<size_t, list_size>{});
    }

    template<typename Action>
    static auto apply(Action&& action) {
        return [&](auto&&... args) {
            for_each([&]<typename T>(size_t i, T) {
                action(i, T{}, args...);
            });
        };
    }

    template<typename Action>
    static auto apply_to_combinations(Action&& action) {
        return [&](auto&&... args) {
            for_each([&]<typename First>(size_t i, First) {
                for_each([&]<typename Second>(size_t j, Second) {
                    action(i, First{}, j, Second{}, args...);
                });
            });
        };
    }

};

template<typename TL>
concept TypeList = requires(TL t)
{
    ([]<typename... Ts> (TList<Ts...>) {})(t);
};

template<typename Predicate, typename Action>
auto do_if(Predicate&& p, Action&& op) {
    return [p, op] (auto&& ... args) {
        if (p(args...)) {
            return op(args...);
        }
    };
}

template<typename TypeTransformer, typename... TTs>
using apply_to_all = TList< typename TypeTransformer::template make<TTs> ... >;

template<TypeList TL, typename TypeTransformer>
using transform = decltype( [] <typename... Ts> (TList<Ts...>) {
    return apply_to_all<TypeTransformer, Ts...>{};
}(TL{}));

struct test_no_transform {
    template<typename T>
    using make = T;
};
static_assert(std::is_same_v<
                      transform<TList<bool, int>, test_no_transform>,
                      TList<bool, int>
>);

