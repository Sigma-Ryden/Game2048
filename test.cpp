#include <iostream>

#include "game2048.hpp"

Game2048::Option get_option(char button)
{
    switch(button)
    {
    case 'w':
        return Game2048::Option::up;

    case 's':
        return Game2048::Option::down;

    case 'a':
        return Game2048::Option::left;

    case 'd':
        return Game2048::Option::right;

    default:
        return Game2048::Option::null;
    }
}

void display(const Game2048& game)
{
    std::cout << "score: " << game.score() << '\n';

    for(std::size_t i = 0; i < game.shape(); ++i, std::cout << '\n')
        for(std::size_t j = 0; j < game.shape(); ++j)
            std::cout << '\t' << game.data(i, j);

    std::cout << '\n';
}

void play()
{
    Game2048 game(4);

    game.reset();
    display(game);

    char button;
    while(!game.done())
    {

        std::cout << "Select option: up(w), down(s), left(a), right(d): ";
        std::cin >> button;

        game.step(get_option(button));
        display(game);
    }

    std::cout << "Game Over!\n";
}

int main()
{
    play();

    return 0;
}
