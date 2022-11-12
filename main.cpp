#include <iostream>
#include <utility>
#include <optional>

#include "tlist.hpp"
#include "shape.hpp"

struct Paper;
struct Scissors;
struct Rock;

struct Rock : Beats<Scissors>, Label<"Rock">, Symbol<'r'> {};
struct Paper : Beats<Rock>, Label<"Paper">, Symbol<'p'> {};
struct Scissors  : Beats<Paper>, Label<"Scissors">, Symbol<'s'> {};

template<Shape LF, Shape RF>
constexpr bool operator>(LF, RF) { return LF::beats::template contains_v<RF>; }

template<Shape LF, Shape RF>
constexpr bool operator==(LF, RF) { return std::is_same_v<RF, LF>; }

using Shapes = TList<Paper, Scissors, Rock>;

struct GameHistory {
    size_t gameTotal = 0;
    size_t userWins = 0;
    size_t compWins = 0;
};

template<Shape UserShape, Shape CompShape>
constexpr auto decide_winner(UserShape, CompShape, GameHistory& history) {
    ++history.gameTotal;
    if (UserShape{} == CompShape{}) {
        return std::tuple{ "same as", "Tie!" };
    }
    else if (UserShape{} > CompShape{}) {
        ++history.userWins;
        return std::tuple{ "beats", "User wins!" };
    }
    else {
        ++history.compWins;
        return std::tuple{ "is beaten by", "Comp wins!" };
    }
}

template<typename T>  requires std::is_same_v<T, char>
char get_symbol_from_input(T c) { return c; }
char get_symbol_from_input(size_t index) {
    char symbol = Shapes::map_reduce(
    [&]<Shape CurrentShape>(size_t i, CurrentShape) -> std::optional<char> {
        if (i == index) return std::optional(CurrentShape::symbol);
        else return std::nullopt;
    },
    [&](std::optional<char> first, std::optional<char> second) -> std::optional<char> {
        if (first) return first;
        else if (second) return second;
        else return std::nullopt;
    },
    std::optional<char>(std::nullopt)
    ).value();
    return symbol;
}

int main() {
    GameHistory history;

    auto game_engine = Shapes::apply_to_combinations([] <Shape UserShape, Shape CompShape>
        (size_t, UserShape user, size_t, CompShape comp, auto user_choice, auto comp_choice, GameHistory& history) {
            auto user_symbol = get_symbol_from_input(user_choice);
            auto comp_symbol = get_symbol_from_input(comp_choice);
            if (UserShape::symbol == user_symbol
                && CompShape::symbol == comp_symbol) {
                auto [sep, result] = decide_winner(user, comp, history);
                std::cout << user.get_label() << " " << sep << " " << comp.get_label() << ". " << result << std::endl;
            }
        }
    );

    game_engine('r', 's', history);
    game_engine('s', 'r', history);
    game_engine('s', 's', history);

    game_engine(0, 1, history);
    game_engine(1, 1, history);
    game_engine(2, 1, history);
}
