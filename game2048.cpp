#include "game2048.hpp"

Game2048::Game2048(size_type side_size)
    : generator_(std::time(nullptr))
    , data_(new unsigned_type [side_size * side_size])
    , buff_(new unsigned_type [side_size])
    , length_(side_size * side_size)
    , shape_(side_size)
    , score_(0)
    , done_(true)
{
}

Game2048::~Game2048()
{
    delete[] data_;
}

void Game2048::reset() noexcept
{
    for(size_type i = 0; i < length_; ++i)
        data_[i] = 0;

    score_ = 0;
    done_  = false;

    random_insert_value();
    random_insert_value();
}

void Game2048::step(Game2048::Option option) noexcept
{
    if(done_) return;

    switch(option)
    {
    case(Option::up):
        for(size_type j = 0; j < shape_; drop_vertical(j, to_up), ++j)
        {
            drop_vertical(j, to_up);
            for(size_type i = 0; i < shape_ - 1; ++i)
                join_vertical(i, j, move_up);
        }
        break;

    case(Option::down):
        for(size_type j = 0; j < shape_; drop_vertical(j, to_down), ++j)
        {
            drop_vertical(j, to_down);
            for(size_type i = shape_ - 1; i > 0; --i)
                join_vertical(i, j, move_down);
        }
        break;

    case(Option::left):
        for(size_type i = 0; i < shape_; drop_horizontal(i, to_up), ++i)
        {
            drop_horizontal(i, to_up);
            for(size_type j = 0; j < shape_ - 1; ++j)
                join_horizontal(i, j, move_up);
        }
        break;

    case(Option::right):
        for(size_type i = 0; i < shape_; drop_horizontal(i, to_down), ++i)
        {
            drop_horizontal(i, to_down);
            for(size_type j = shape_ - 1; j > 0; --j)
                join_horizontal(i, j, move_down);
        }
        break;

    default:
        return;
    }

    random_insert_value();
}

std::size_t Game2048::generate_insert_value(double probability_generate_2) noexcept
{
    return (generator_() % 101 < static_cast<size_type>(probability_generate_2 * 100)) ? 2 : 4;
}

void Game2048::random_insert_value() noexcept
{
    if(done_) return;

    size_type n = 0;

    for(size_type i = 0; i < length_; ++i)
        if(data_[i] == 0) ++n;

    if(n == 0)
    {
        done_ = true;
        return;
    }

    while(true)
    {
        n = generator_() % length_;

        if(data_[n] == 0)
        {
            data_[n] = generate_insert_value();
            return;
        }
    }
}

void Game2048::join_vertical(
    size_type i,
    size_type j,
    unsigned_type (*vertical_move)(unsigned_type)) noexcept
{
    if(data(i, j) == data(vertical_move(i), j))
    {
        data(i, j) <<= 1;
        data(vertical_move(i), j) = 0;

        score_ += data(i, j);
    }
}

void Game2048::join_horizontal(
    size_type i,
    size_type j,
    unsigned_type (*horizontal_move)(unsigned_type)) noexcept
{
    if(data(i, j) == data(i, horizontal_move(j)))
    {
        data(i, j) <<= 1;
        data(i, horizontal_move(j)) = 0;

        score_ += data(i, j);
    }
}

void Game2048::drop_vertical(
    size_type j,
    bool (*to_vertical_side)(unsigned_type, unsigned_type)) noexcept
{
    for(size_type i = 0; i < shape_; ++i)
        buff_[i] = data(i, j);

    std::sort(buff_, buff_ + shape_, to_vertical_side);

    for(size_type i = 0; i < shape_; ++i)
        data(i, j) = buff_[i];
}

void Game2048::drop_horizontal(
    size_type i,
    bool (*to_horizontal_side)(unsigned_type, unsigned_type)) noexcept
{
    std::sort(data_ + i * shape_, data_ + (i + 1) * shape_, to_horizontal_side);
}
