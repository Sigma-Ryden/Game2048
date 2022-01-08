#include "game2048.hpp"

Game2048::Game2048(size_type side_size)
    : generator_(std::time(nullptr))
    , data_(new unsigned_type [side_size * side_size])
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

void Game2048::step(Game2048::Option option) noexcept
{
    if(done_) return;

    switch(option)
    {
    case(Option::up):
        option_inc(&Game2048::vertical_access);
        break;

    case(Option::down):
        option_dec(&Game2048::vertical_access);
        break;

    case(Option::left):
        option_inc(&Game2048::horizontal_access);
        break;

    case(Option::right):
        option_dec(&Game2048::horizontal_access);
        break;

    default:
        return;
    }

    random_insert_value();
}

void Game2048::option_inc(
    unsigned_type &(Game2048::*data_at)(size_type, size_type)) noexcept
{
    for(size_type n = 0; n < shape_; drop_zero_right(n, data_at), ++n)
    {
        drop_zero_right(n, data_at);
        for(size_type i = 0; i < shape_ - 1; ++i)
            join_tail(n, i, move_inc, data_at);
    }
}

void Game2048::option_dec(
    unsigned_type &(Game2048::*data_at)(size_type, size_type)) noexcept
{
    for(size_type n = 0; n < shape_; drop_zero_left(n, data_at), ++n)
    {
        drop_zero_left(n, data_at);
        for(size_type i = shape_ - 1; i > 0; --i)
            join_tail(n, i, move_dec, data_at);
    }
}

void Game2048::join_tail(
    size_type n,
    size_type k,
    unsigned_type (*move_tail)(unsigned_type),
    unsigned_type& (Game2048::*data_at)(size_type, size_type)) noexcept
{
    if((this->*data_at)(n, move_tail(k)) == (this->*data_at)(n, k))
    {
        (this->*data_at)(n, move_tail(k)) = 0;
        (this->*data_at)(n, k)          <<= 1;

        score_ += (this->*data_at)(n, k);
    }
}

void Game2048::drop_zero_left(
    size_type n,
    unsigned_type& (Game2048::*data_at)(size_type, size_type)) noexcept
{
    int beg = 0;
    int end = shape_ - 1;
    int i;

    while(beg < end)
    {
        if((this->*data_at)(n, end) == 0)
        {
            for(i = end; i > beg; --i)
                std::swap((this->*data_at)(n, i), (this->*data_at)(n, i - 1));

            ++beg;
        }
        else
        {
            --end;
        }
    }
}

void Game2048::drop_zero_right(
    size_type n,
    unsigned_type& (Game2048::*data_at)(size_type, size_type)) noexcept
{
    int beg = 0;
    int end = shape_ - 1;

    int i;

    while(beg < end)
    {
        if((this->*data_at)(n, beg) == 0)
        {
            for(i = beg; i < end; ++i)
                std::swap((this->*data_at)(n, i), (this->*data_at)(n, i + 1));

            --end;
        }
        else
        {
            ++beg;
        }
    }
}
