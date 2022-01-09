#ifndef GAME2048_HPP
#define GAME2048_HPP

#include <cstdint> // uint8_t, size_t
#include <ctime> // time
#include <random> // mt19937_64
#include <utility> // swap

class Game2048
{
public:
    using Generator                  = std::mt19937_64;

    using byte_type                  = std::uint8_t;
    using size_type                  = std::size_t;

    using unsigned_type              = std::size_t;

    using MoveTailFunction           = unsigned_type (*)(unsigned_type);
    using Game2048DataAtFunction     = unsigned_type& (Game2048::*)(unsigned_type*, size_type, size_type);
    using Game2048MoveOptionFunction = void (Game2048::*)(size_type&, unsigned_type*, Game2048DataAtFunction);
    using Game2048DropZeroFunction   = void (Game2048::*)(size_type, unsigned_type*, Game2048DataAtFunction);

public:
    enum class Option : byte_type
    {
        null, up, down, left, right
    };

private:
    Generator generator_;

    unsigned_type* data_;
    unsigned_type* buff_;
		
    size_type length_;
    size_type shape_;

    size_type score_;

    bool done_;

public:
    Game2048(size_type side_size);
    ~Game2048();

    void reset() noexcept;

    void step(Option option) noexcept;

    const unsigned_type& data(size_type i, size_type j) const noexcept;
    const unsigned_type* data() const noexcept;

    size_type length() const noexcept { return length_; }
    size_type shape() const noexcept { return shape_; }

    size_type score() const noexcept { return score_; }

    bool done() const noexcept { return done_; }

private:
    unsigned_type& horizontal_access(unsigned_type* ptr,
                                     size_type row_n,
                                     size_type i) noexcept;

    unsigned_type& vertical_access(unsigned_type* ptr,
                                   size_type col_n,
                                   size_type i) noexcept;

    size_type generate_insert_value(double prob_gen_2 = 0.9) noexcept;
    void random_insert() noexcept;
    void update() noexcept;

    bool has_vertical_inc_join() noexcept;
    bool has_vertical_dec_join() noexcept;
    bool has_horizontal_inc_join() noexcept;
    bool has_horizontal_dec_join() noexcept;
	
    bool has_join(Game2048DropZeroFunction,
                  Game2048MoveOptionFunction,
                  Game2048DataAtFunction) noexcept;


    void join_tail(size_type i, size_type j,
                   size_type& score,
                   unsigned_type* ptr,
                   MoveTailFunction move_tail,
                   Game2048DataAtFunction at) noexcept;

    void drop_zero_left(size_type n,
                        unsigned_type* ptr,
                        Game2048DataAtFunction at) noexcept;
    										
    void drop_zero_right(size_type n,
                         unsigned_type* ptr,
                         Game2048DataAtFunction at) noexcept;
    										 
    void option_inc(size_type& score,
                    unsigned_type* ptr,
                    Game2048DataAtFunction at) noexcept;
    								
    void option_dec(size_type& score,
                    unsigned_type* ptr,
                    Game2048DataAtFunction at) noexcept;

private:
    static size_type move_inc(unsigned_type a) noexcept;
    static size_type move_dec(unsigned_type a) noexcept;
};

inline const std::size_t& Game2048::data(
    std::size_t i, std::size_t j) const noexcept
{
    return data_[i * shape_ + j];
}

inline const std::size_t* Game2048::data() const noexcept
{
    return data_;
}

inline std::size_t& Game2048::horizontal_access(
    unsigned_type* ptr,
    std::size_t row_n,
    std::size_t i) noexcept
{
    return ptr[row_n * shape_ + i];
}

inline std::size_t& Game2048::vertical_access(
    unsigned_type* ptr,
    std::size_t col_n,
    std::size_t i) noexcept
{
    return ptr[i * shape_ + col_n];
}

inline std::size_t Game2048::move_inc(std::size_t a) noexcept
{
    return ++a;
}

inline std::size_t Game2048::move_dec(std::size_t a) noexcept
{
    return --a;
}

#endif // GAME2048_HPP
