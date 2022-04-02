#include "game2048.hpp"

#define access(number_of, position) (this->*at)(number_of, position)

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
    Game2048::copy(data_, data_ + full_size_, data);
}

Game2048::Game2048(const Game2048& game2048)
    : generator_(new Generator(*game2048.generator_))
    , data_(new tile_value_type [game2048.full_size_])
    , full_size_(game2048.full_size_)
    , size_(game2048.size_)
    , score_(game2048.score_)
    , done_(game2048.done_)
{
    Game2048::copy(data_, data_ + full_size_, game2048.data_);
}

Game2048::Game2048(Game2048&& game2048) noexcept
    : generator_(game2048.generator_)
    , data_(game2048.data_)
    , full_size_(game2048.full_size_)
    , size_(game2048.size_)
    , score_(game2048.score_)
    , done_(game2048.done_)
{
    game2048.generator_ = nullptr;
    game2048.data_ = nullptr;
}

Game2048::~Game2048()
{
    delete generator_;
    delete[] data_;
}

Game2048& Game2048::operator= (const Game2048& game2048)
{
    if(this != &game2048)
    {
        delete[] data_;
        delete generator_;

        generator_ = new Generator(*game2048.generator_);
        data_ = new tile_value_type [game2048.full_size_];

        full_size_ = game2048.full_size_;
        size_ = game2048.size_;
        score_ = game2048.score_;
        done_ = game2048.done_;

        Game2048::copy(data_, data_ + full_size_, game2048.data_);
    }

    return *this;
}

Game2048& Game2048::operator= (Game2048&& game2048) noexcept
{
    if(this != &game2048)
    {
        delete[] data_;
        delete generator_;

        generator_ = game2048.generator_;
        data_ = game2048.data_;

        full_size_ = game2048.full_size_;
        size_ = game2048.size_;
        score_ = game2048.score_;
        done_ = game2048.done_;

        game2048.generator_ = nullptr;
        game2048.data_ = nullptr;
    }

    return *this;
}

void Game2048::reset() noexcept
{
    Game2048::fill(data_, data_ + full_size_, 0);

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

    if(not has_increase_join(&Game2048::vertical_access) and
       not has_decrease_join(&Game2048::vertical_access) and
       not has_increase_join(&Game2048::horizontal_access) and
       not has_decrease_join(&Game2048::horizontal_access))
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

bool Game2048::step(Game2048::Option option) noexcept
{
    if(done_) return false;

    bool has_join_up    = has_increase_join(&Game2048::vertical_access);
    bool has_join_down  = has_decrease_join(&Game2048::vertical_access);
    bool has_join_left  = has_increase_join(&Game2048::horizontal_access);
    bool has_join_right = has_decrease_join(&Game2048::horizontal_access);
   
    if(not has_join_up and
       not has_join_down and
       not has_join_left and
       not has_join_right)
    {
        done_ = true;
        return false;
    }
    
    switch(option)
    {
    case(Option::up):
        if(not has_join_up) return false;
      	
        option_increase(&Game2048::vertical_access);
      	break;

    case(Option::down):
        if(not has_join_down) return false;
      	
        option_decrease(&Game2048::vertical_access);
      	break;

    case(Option::left):
        if(not has_join_left) return false;
      	
        option_increase(&Game2048::horizontal_access);
      	break;

    case(Option::right):
        if(not has_join_right) return false;
        
        option_decrease(&Game2048::horizontal_access);
        break;

    default:
        return false;
    }

    update();

    return true;
}

void Game2048::join_tail(
    size_type n, size_type k,
    FuncMoveTile move_tile,
    MFuncDataAccess at) noexcept
{
    if((this->*at)(n, move_tile(k)) == (this->*at)(n, k))
    {
        (this->*at)(n, move_tile(k)) = 0;
        (this->*at)(n, k)           *= 2;

        score_ += (this->*at)(n, k);
    }
}

void Game2048::option_increase(MFuncDataAccess at) noexcept
{
    for(size_type n = 0; n < size_; drop_zero_right(n, at), ++n)
    {
        drop_zero_right(n, at);
        for(size_type i = 0; i < size_ - 1; ++i)
            join_tail(n, i, move_increase, at);
    }
}

void Game2048::option_decrease(MFuncDataAccess at) noexcept
{
    for(size_type n = 0; n < size_; drop_zero_left(n, at), ++n)
    {
        drop_zero_left(n, at);
        for(size_type i = size_ - 1; i > 0; --i)
            join_tail(n, i, move_decrease, at);
    }
}

void Game2048::drop_zero_left(size_type n, MFuncDataAccess at) noexcept
{
    size_type first = 0;
    size_type last = size_ - 1;

    size_type i;

    // last is never less than 0
    while(first < last)
    {
        if(access(n, last) == 0)
        {
            for(i = last; i > first; --i)
                std::swap(access(n, i), access(n, i - 1));

            ++first;
        }

        else --last;
    }
}

void Game2048::drop_zero_right(size_type n, MFuncDataAccess at) noexcept
{
    size_type first = 0;
    size_type last = size_ - 1;

    size_type i;

    // last is never less than 0
    while(first < last)
    {
        if(access(n, first) == 0)
        {
            for(i = first; i < last; ++i)
                std::swap(access(n, i), access(n, i + 1));

            --last;
        }

        else ++first;
    }
}

bool Game2048::has_increase_join(MFuncDataAccess at) noexcept
{
    for(size_type n = 0; n < size_; ++n)
        if(check_increase(n, at))
            return true;

    return false;
}

bool Game2048::has_decrease_join(MFuncDataAccess at) noexcept
{
    for(size_type n = 0; n < size_; ++n)
        if(check_decrease(n, at))
            return true;

    return false;
}

bool Game2048::check_increase(size_type n, MFuncDataAccess at) noexcept
{
    size_type lhs = size_ - 2;
    for(size_type rhs = size_ - 1; rhs > 0; --lhs)
    {
        if(access(n, rhs) != 0 and (
                access(n, rhs) == access(n, lhs) or access(n, lhs) == 0
            )
        ) return true;

        else rhs = lhs;
    }

    return false;
}

bool Game2048::check_decrease(size_type n, MFuncDataAccess at) noexcept
{
    size_type lhs = 0;
    for(size_type rhs = 1; rhs < size_; ++rhs)
    {
        if(access(n, lhs) != 0 and (
                access(n, lhs) == access(n, rhs) or access(n, rhs) == 0
            )
        ) return true;

        else lhs = rhs;
    }

    return false;
}

void Game2048::copy(pointer first, pointer last, const_pointer src) noexcept
{
    while(first != last) *first++ = *src++;
}

void Game2048::fill(pointer first, pointer last, tile_value_type value) noexcept
{
    while(first != last) *first++ = value;
}

// clean up
#undef access
