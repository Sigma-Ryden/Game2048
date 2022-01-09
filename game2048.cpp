#include "game2048.hpp"

Game2048::Game2048(size_type side_size)
    : generator_(std::time(nullptr))
    , data_(new unsigned_type [side_size * side_size])
    , buff_(new unsigned_type [side_size * side_size])
    , length_(side_size * side_size)
    , shape_(side_size)
    , score_(0)
    , done_(true)
{
}

Game2048::~Game2048()
{
    delete[] data_;
    delete[] buff_;
}

void Game2048::reset() noexcept
{
    size_type n;
    for(n = 0; n < length_; ++n)
        data_[n] = 0;

    score_ = 0;
    done_  = false;

    random_insert();
    random_insert();
}

std::size_t Game2048::generate_insert_value(double prob_gen_2) noexcept
{
    return (generator_() % 100 < static_cast<size_type>(prob_gen_2 * 100)) ? 2 : 4;
}

void Game2048::random_insert() noexcept
{
    size_type n;
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

void Game2048::update() noexcept
{
    if(done_) return;
   
    if(!has_vertical_inc_join() && !has_vertical_dec_join() &&
       !has_horizontal_inc_join() && !has_horizontal_dec_join())
    {
        done_ = true;
        return;
    }
    	
    size_type n = 0;
    for(size_type i = 0; i < length_; ++i)
        if(data_[i] == 0) ++n;
       
    if(n != 0) random_insert();
}

void Game2048::step(Game2048::Option option) noexcept
{
    if(done_) return;

    bool has_join_up    = has_vertical_inc_join();
    bool has_join_down  = has_vertical_dec_join();
    bool has_join_left  = has_horizontal_inc_join();
    bool has_join_right = has_horizontal_dec_join();
   
    if(!has_join_up && !has_join_down &&
       !has_join_left && !has_join_right)
    {
        done_ = true;
        return;
    }
    
    switch(option)
    {
    case(Option::up):
        if(!has_join_up) return;
      	
      	option_inc(score_, data_, &Game2048::vertical_access);
      	break;

    case(Option::down):
      	if(!has_join_down) return;
      	
        option_dec(score_, data_, &Game2048::vertical_access);
      	break;

    case(Option::left):
      	if(!has_join_left) return;
      	
      	option_inc(score_, data_, &Game2048::horizontal_access);
      	break;

    case(Option::right):
        if(!has_join_right) return;
        
        option_dec(score_, data_, &Game2048::horizontal_access);
        break;

    default:
        return;
    }

    update();
}

void Game2048::option_inc(
    size_type& score,
    unsigned_type* ptr,
    Game2048DataAtFunction at) noexcept
{
    for(size_type n = 0; n < shape_; drop_zero_right(n, ptr, at), ++n)
    {
        drop_zero_right(n, ptr, at);
        for(size_type i = 0; i < shape_ - 1; ++i)
            join_tail(n, i, score, ptr, move_inc, at);
    }
}

void Game2048::option_dec(
    size_type& score,
    unsigned_type* ptr,
    Game2048DataAtFunction at) noexcept
{
    for(size_type n = 0; n < shape_; drop_zero_left(n, ptr, at), ++n)
    {
        drop_zero_left(n, ptr, at);
        for(size_type i = shape_ - 1; i > 0; --i)
            join_tail(n, i, score, ptr, move_dec, at);
    }
}

void Game2048::join_tail(
    size_type n,
    size_type k,
    size_type& score,
    unsigned_type* ptr,
    MoveTailFunction move_tail,
    Game2048DataAtFunction at) noexcept
{
    if((this->*at)(ptr, n, move_tail(k)) == (this->*at)(ptr, n, k))
    {
        (this->*at)(ptr, n, move_tail(k)) = 0;
        (this->*at)(ptr, n, k)          <<= 1;

        score += (this->*at)(ptr, n, k);
    }
}

void Game2048::drop_zero_left(
    size_type n,
    unsigned_type* ptr,
    Game2048DataAtFunction at) noexcept
{
    int beg = 0;
    int end = shape_ - 1;
    int i;

    while(beg < end)
    {
        if((this->*at)(ptr, n, end) == 0)
        {
            for(i = end; i > beg; --i)
                std::swap((this->*at)(ptr, n, i), (this->*at)(ptr, n, i - 1));

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
    unsigned_type* ptr,
    Game2048DataAtFunction at) noexcept
{
    int beg = 0;
    int end = shape_ - 1;

    int i;

    while(beg < end)
    {
        if((this->*at)(ptr, n, beg) == 0)
        {
            for(i = beg; i < end; ++i)
                std::swap((this->*at)(ptr, n, i), (this->*at)(ptr, n, i + 1));

            --end;
        }
        else
        {
            ++beg;
        }
    }
}

bool Game2048::has_vertical_inc_join() noexcept
{
    return has_join(&Game2048::drop_zero_right,
                    &Game2048::option_inc,
                    &Game2048::vertical_access);
}

bool Game2048::has_vertical_dec_join() noexcept
{
    return has_join(&Game2048::drop_zero_left,
                    &Game2048::option_dec,
                    &Game2048::vertical_access);
}

bool Game2048::has_horizontal_inc_join() noexcept
{		 	
    return has_join(&Game2048::drop_zero_right,
                    &Game2048::option_inc,
                    &Game2048::horizontal_access);
}

bool Game2048::has_horizontal_dec_join() noexcept
{		 	
    return has_join(&Game2048::drop_zero_left,
                    &Game2048::option_dec,
                    &Game2048::horizontal_access);
}

bool Game2048::has_join(
    Game2048DropZeroFunction drop_zero,
    Game2048MoveOptionFunction option,
    Game2048DataAtFunction at) noexcept
{
    for(size_type i = 0; i < length_; ++i)
        buff_[i] = data_[i];
    
    size_type unused = 0;
    bool flag = false;
    
    for(size_type n = 0; n < shape_; ++n)
    {
        (this->*drop_zero)(n, buff_, at);
        (this->*option)(unused, buff_, at);

        for(size_type i = 0; i < shape_; ++i)
        {
            if((this->*at)(buff_, n, i) != (this->*at)(data_, n, i))
            {
                flag = true;
                break;
            }
        }

        if(flag) break;
    }
 	
    return flag;
}
