#include "game2048.hpp"

Game2048::Game2048(
    size_type side_size)
    : generator_(new Generator(std::time(nullptr)))
    , data_(new tile_value_type [side_size * side_size])
    , full_size_(side_size * side_size)
    , size_(side_size)
    , score_(0)
    , done_(true)
{
}

Game2048::Game2048(
    size_type side_size,
    const_pointer data,
    size_type score,
    bool done)
    : generator_(new Generator(std::time(nullptr)))
    , data_(new tile_value_type [side_size * side_size])
    , full_size_(side_size * side_size)
    , size_(side_size)
    , score_(score)
    , done_(done)
{
    for(size_type i = 0; i < size_; ++i)
        data_[i] = data[i];
}

Game2048::~Game2048()
{
    delete generator_;
    delete[] data_;
}

void Game2048::reset() noexcept
{
    for(size_type n = 0; n < full_size_; ++n)
        data_[n] = 0;

    score_ = 0;
    done_  = false;

    // at the beginning of new game we should be generate two numbers on the grid
    random_insert();
    random_insert();
}

Game2048::tile_value_type Game2048::generate_insert_value(double gen2) noexcept
{
    return (*generator_)() % 100 < static_cast<size_type>(gen2 * 100) ? 2 : 4;
}

void Game2048::random_insert() noexcept
{
    size_type random_position;
    while(true)
    {
        random_position = (*generator_)() % full_size_;

        if(data_[random_position] == 0)
        {
            data_[random_position] = generate_insert_value();
            return;
        }
    }
}

void Game2048::update() noexcept
{
    if(done_) return;

    if(not has_inc_join(&Game2048::vertical_access) and
       not has_dec_join(&Game2048::vertical_access) and
       not has_inc_join(&Game2048::horizontal_access) and
       not has_dec_join(&Game2048::horizontal_access))
    {
        done_ = true;
        return;
    }

    for(size_type i = 0; i < full_size_; ++i)
    {
        if(data_[i] == 0)
        {
            random_insert();
            return;
        }
    }
}

void Game2048::step(Game2048::Option option) noexcept
{
    if(done_) return;

    bool has_join_up    = has_inc_join(&Game2048::vertical_access);
    bool has_join_down  = has_dec_join(&Game2048::vertical_access);
    bool has_join_left  = has_inc_join(&Game2048::horizontal_access);
    bool has_join_right = has_dec_join(&Game2048::horizontal_access);
   
    if(not has_join_up and
       not has_join_down and
       not has_join_left and
       not has_join_right)
    {
        done_ = true;
        return;
    }
    
    switch(option)
    {
    case(Option::up):
        if(not has_join_up) return;
      	
        option_inc(&Game2048::vertical_access);
      	break;

    case(Option::down):
      	if(not has_join_down) return;
      	
        option_dec(&Game2048::vertical_access);
      	break;

    case(Option::left):
      	if(not has_join_left) return;
      	
        option_inc(&Game2048::horizontal_access);
      	break;

    case(Option::right):
        if(not has_join_right) return;
        
        option_dec(&Game2048::horizontal_access);
        break;

    default:
        return;
    }

    update();
}

void Game2048::join_tail(
    size_type n, size_type k,
    FuncMoveTile move_tile,
    MFuncDataAt at) noexcept
{
    if((this->*at)(n, move_tile(k)) == (this->*at)(n, k))
    {
        (this->*at)(n, move_tile(k)) = 0;
        (this->*at)(n, k)           *= 2;

        score_ += (this->*at)(n, k);
    }
}

void Game2048::option_inc(MFuncDataAt at) noexcept
{
    for(size_type n = 0; n < size_; drop_zero_right(n, at), ++n)
    {
        drop_zero_right(n, at);
        for(size_type i = 0; i < size_ - 1; ++i)
            join_tail(n, i, move_inc, at);
    }
}

void Game2048::option_dec(MFuncDataAt at) noexcept
{
    for(size_type n = 0; n < size_; drop_zero_left(n, at), ++n)
    {
        drop_zero_left(n, at);
        for(size_type i = size_ - 1; i > 0; --i)
            join_tail(n, i, move_dec, at);
    }
}

void Game2048::drop_zero_left(size_type n, MFuncDataAt at) noexcept
{
    size_type beg = 0;
    size_type end = size_ - 1;

    size_type i;

    // end is never less than 0
    while(beg < end)
    {
        if((this->*at)(n, end) == 0)
        {
            for(i = end; i > beg; --i)
                std::swap((this->*at)(n, i), (this->*at)(n, i - 1));

            ++beg;
        }
        else
        {
            --end;
        }
    }
}

void Game2048::drop_zero_right(size_type n, MFuncDataAt at) noexcept
{
    size_type beg = 0;
    size_type end = size_ - 1;

    size_type i;

    // end is never less than 0
    while(beg < end)
    {
        if((this->*at)(n, beg) == 0)
        {
            for(i = beg; i < end; ++i)
                std::swap((this->*at)(n, i), (this->*at)(n, i + 1));

            --end;
        }
        else
        {
            ++beg;
        }
    }
}

bool Game2048::has_inc_join(MFuncDataAt at) noexcept
{
    for(size_type n = 0; n < size_; ++n)
        if(check_inc(n, at))
            return true;

    return false;
}

bool Game2048::has_dec_join(MFuncDataAt at) noexcept
{
    for(size_type n = 0; n < size_; ++n)
        if(check_dec(n, at))
            return true;

    return false;
}

bool Game2048::check_inc(size_type n, MFuncDataAt at) noexcept
{
    size_type lhs = size_ - 2;
    for(size_type rhs = size_ - 1; rhs > 0; --lhs)
    {
        if((this->*at)(n, rhs) != 0 and (
                (this->*at)(n, lhs) == 0 or (this->*at)(n, rhs) == (this->*at)(n, lhs)
            )
        ) return true;

        else rhs = lhs;
    }

    return false;
}

bool Game2048::check_dec(size_type n, MFuncDataAt at) noexcept
{
    size_type lhs = 0;
    for(size_type rhs = 1; rhs < size_; ++rhs)
    {
        if((this->*at)(n, lhs) != 0 and (
                (this->*at)(n, rhs) == 0 or (this->*at)(n, lhs) == (this->*at)(n, rhs)
            )
        ) return true;

        else lhs = rhs;
    }

    return false;
}
