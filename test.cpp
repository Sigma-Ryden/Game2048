#include <iostream>

#include "game2048.hpp"

using std::cout;
using std::cin;

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
    using size_type = Game2048::size_type;

    cout << "score: " << game.score() << '\n';

    for(size_type i = 0; i < game.size(); ++i, cout << '\n')
        for(size_type j = 0; j < game.size(); ++j)
            cout << '\t' << game.data(i, j);

    cout << '\n';
}

void play()
{
    Game2048 game(4);

    game.reset();
    display(game);

    char button;
    while(not game.done())
    {

        cout << "Select option: up(w), down(s), left(a), right(d): ";
        cin >> button;

        game.step(get_option(button));
        display(game);
    }

    cout << "Game Over!\n";
}

int main()
{
    play();

    return 0;
}
