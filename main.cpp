#include <iostream>
#include <format>
#include <string_view>
#include <array>

class Game_Board
{
public:
    bool finished;

    Game_Board()
    {
        reset();
    }

    void reset()
    {
        for (auto& slot : slots)
        {
            slot = Symbol::Empty;
        }
        filled_slot_count = 0;

        state = State::Player1_Turn;
        
        finished = false;
    }

    void print()
    {
        for (size_t i = 0; i < SLOT_COUNT; i += ROW_COUNT)
        {
            std::cout << std::format(" {} | {} | {} \n",
                symbol_to_char(slots[i]), symbol_to_char(slots[i+1]), symbol_to_char(slots[i+2]));

            if (i != SLOT_COUNT - ROW_COUNT)
            {
                std::cout << ROW_DIVIDER;
            }
        }

        switch (state)
        {
            case State::Player1_Turn:
                std::cout << "Player 1 (X), make a move!\n";
                break;
            case State::Player2_Turn:
                std::cout << "Player 2 (O), make a move!\n";
                break;
        }
    }

    void input(size_t index)
    {
        index -= 1;

        if (index < 0 || index > 8)
        {
            std::cout << "Out of bounds! Choose a value between 1-9!\n";
            return;
        }

        if (slots[index] != Symbol::Empty)
        {
            std::cout << "That spot is taken! Try a different one...\n";
            return;
        }

        switch (state)
        {
            case State::Player1_Turn:
                slots[index] = Symbol::X;
                break;
            case State::Player2_Turn:
                slots[index] = Symbol::O;
                break;
        }

        change_state();
    }

private:
    enum class Symbol
    {
        Empty, X, O
    };

    enum class State
    {
        Player1_Turn,
        Player2_Turn,
        Player1_Win,
        Player2_Win,
        Stalemate,
    };

    static const size_t SLOT_COUNT = 9;
    static const size_t ROW_COUNT = 3;
    const std::string_view ROW_DIVIDER = "---+---+---\n";

    std::array<Symbol, SLOT_COUNT> slots;
    size_t filled_slot_count;

    State state;

    static constexpr std::array<std::array<size_t, 3>, 8> WINNING_COMBOS =
    {{
        {{1, 4, 7}},
        {{2, 5, 8}},
        {{3, 6, 9}},
        {{1, 2, 3}},
        {{4, 5, 6}},
        {{7, 8, 9}},
        {{1, 5, 9}},
        {{3, 5, 7}},
    }};

    [[nodiscard]] static constexpr char symbol_to_char(const Symbol symbol)
    {
        switch (symbol)
        {
            case Symbol::X: return 'X';
            case Symbol::O: return 'O';
        }
        return ' ';
    }

    void check_win()
    {
        for (int i = 0; i < 8; i++)
        {
            Symbol current_symbol = slots[WINNING_COMBOS[i][0]-1];

            if (current_symbol == Symbol::Empty)
            {
                continue;
            }

            if (slots[WINNING_COMBOS[i][1]-1] == current_symbol && slots[WINNING_COMBOS[i][2]-1] == current_symbol)
            {
                if (current_symbol == Symbol::X)
                {
                    state = State::Player1_Win;
                }
                else
                {
                    state = State::Player2_Win;
                }
            }
        }
    }

    void change_state()
    {
        check_win();

        switch (state)
        {
            case State::Player1_Turn:
                state = State::Player2_Turn;
                filled_slot_count++;
                break;
            case State::Player2_Turn:
                state = State::Player1_Turn;
                filled_slot_count++;
                break;
            case State::Player1_Win:
                std::cout << "PLAYER 1 (X) WINS! Congratulations!\n";
                finished = true;
                return;
            case State::Player2_Win:
                std::cout << "PLAYER 2 (O) WINS! Well played!\n";
                finished = true;
                return;
        }

        if (filled_slot_count >= 9)
        {
            state = State::Stalemate;
            std::cout << "STALEMATE! No player wins!\n";
            finished = true;
        }
    }
};

class Player_AI
{
    // TBD
};

int main()
{
    Game_Board board;
    std::string input;

    std::cout << "Welcome to Terminal Tic-Tac-Toe!\n"
        << "Send \"exit\" or \"quit\" at any time to stop playing.\n\n";

    std::cout << "Would you like to play local PvP (1) or against AI (2)?\n";
    std::cout << "> ";
    std::cin >> input;
    std::cout << "\n";

    while (true)
    {
        board.print();

        if (board.finished)
        {
            break;
        }

        std::cout << "> ";
        std::cin >> input;
        std::cout << "\n";

        if (input == "exit" || input == "e" || input == "quit" || input == "q")
        {
            break;
        }
        
        board.input(std::stoi(input));
    }
}