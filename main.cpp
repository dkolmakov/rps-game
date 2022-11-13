#include <iostream>
#include <optional>
#include <set>
#include <random>

#include "tlist.hpp"
#include "shape.hpp"

struct Paper;
struct Scissors;
struct Rock;

struct Rock : Beats<Scissors>, Label<"Rock">, Symbol<'r'> {};
struct Paper : Beats<Rock>, Label<"Paper">, Symbol<'p'> {};
struct Scissors  : Beats<Paper>, Label<"Scissors">, Symbol<'s'> {};

using Shapes = TList<Paper, Scissors, Rock>;

constexpr auto index_to_symbols_tbl = Shapes::map(
        []<Shape CurrentShape>(CurrentShape) { return CurrentShape::symbol; }
);

constexpr auto index_to_label_tbl = Shapes::map(
        []<Shape CurrentShape>(CurrentShape) { return CurrentShape::get_label(); }
);

auto index_to_beaten_shapes_tbl = Shapes::map(
        []<Shape CurrentShape>(CurrentShape) {
            auto beats = CurrentShape::beats::map(
                    []<Shape BeatenShape>(BeatenShape) { return BeatenShape::symbol; }
            );
            return std::set(beats.begin(), beats.end());
        }
);


bool beats(size_t index, char symbol) {
    return index_to_beaten_shapes_tbl[index].contains(symbol);
}

char index_to_symbol(size_t index) {
    return index_to_symbols_tbl[index];
}

std::optional<size_t> symbol_to_index(char target) {
    for (size_t i = 0; i < index_to_symbols_tbl.size(); ++i) {
        if (index_to_symbols_tbl[i] == target) return i;
    }
    return std::nullopt;
}

std::optional<std::string_view> get_label(char c) {
    auto index = symbol_to_index(c);
    if (index)
        return index_to_label_tbl[*index];
    else
        return std::nullopt;
}

std::optional<std::string_view> get_label(size_t index) {
    if (index < Shapes::list_size)
        return index_to_label_tbl[index];
    else
        return std::nullopt;
}

bool is_valid(char input) {
    return symbol_to_index(input).has_value();
}

int main() {
    struct GameHistory {
        size_t gameTotal = 0;
        size_t userWins = 0;
        size_t compWins = 0;

        void print() const {
            std::cout << "Total games: " << gameTotal << std::endl;
            std::cout << "User won: " << userWins << std::endl;
            std::cout << "Computer won: " << compWins << std::endl;
            std::cout << "Ties: " << gameTotal - userWins - compWins << std::endl;
        }
    } history;

    auto decide_winner = [&history](size_t comp_index, char user_symbol) {
        ++history.gameTotal;
        if (index_to_symbol(comp_index) == user_symbol) {
            return std::tuple{ " same as ", "Tie!" };
        }
        else if (beats(comp_index, user_symbol)) {
            ++history.compWins;
            return std::tuple{ "beats", "Computer wins!" };
        }
        else {
            ++history.userWins;
            return std::tuple{ "is beaten by", "User wins!" };
        }
    };

    auto game_engine = [&decide_winner] (size_t comp_index, char user_symbol) {
        auto user_label = get_label(user_symbol);
        auto comp_label = get_label(comp_index);

        auto [sep, result] = decide_winner(comp_index, user_symbol);
        std::cout << *comp_label << " " << sep << " " << *user_label << ". " << result
                  << std::endl;
    };

    auto help = [] () {
        size_t n_shapes = index_to_symbols_tbl.size();
        for (size_t i = 0; i < n_shapes; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << *get_label(i) << " - " << index_to_symbols_tbl[i];
        }
    };

    std::random_device rd;
    std::uniform_int_distribution dist(0, static_cast<int>(Shapes::list_size) - 1);
    auto generate_comp_choice = [&dist, &rd] {
        return dist(rd);
    };

    char input = 's';
    while (input != 'q') {
        std::cout << "Enter your choice (h for help): ";
        std::cin >> input;

        if (input == 'h') help();
        else if (is_valid(input)) game_engine(generate_comp_choice(), input);
        else std::cout << "Invalid input!" << std::endl;
    }

    history.print();
}
