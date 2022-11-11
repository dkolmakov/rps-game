#include <iostream>
#include <utility>

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
constexpr auto decide_winner(UserShape lf, CompShape rf, GameHistory& history) {
    ++history.gameTotal;
    if (lf == rf) {
        return std::tuple{ "same as", "Tie!" };
    }
    else if (lf > rf) {
        ++history.userWins;
        return std::tuple{ "beats", "User wins!" };
    }
    else {
        ++history.compWins;
        return std::tuple{ "is beaten by", "Comp wins!" };
    }
}

template<typename T>  requires std::is_same_v<T, char>
char get_symbol(T c) { return c; }
char get_symbol(size_t index) {
    char symbol = -1;
    Shapes::for_each([&]<Shape shape>(size_t i, shape) {
        if (i == index) {
            symbol = shape::symbol;
        }
    });
    return symbol;
}

int main() {
    GameHistory history;

    auto game_engine = Shapes::apply_to_combinations([] <Shape UserShape, Shape CompShape>
        (size_t, UserShape user, size_t, CompShape comp, auto user_choice, auto comp_choice, GameHistory& history) {
            if (UserShape::symbol == get_symbol(user_choice) && CompShape::symbol == get_symbol(comp_choice)) {
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
