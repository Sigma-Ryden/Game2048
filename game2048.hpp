#ifndef GAME2048_HPP
#define GAME2048_HPP

#include <cstdint> // uint8_t, size_t
#include <ctime> // time
#include <random> // mt19937_64
#include <utility> // swap

class Game2048
{
public:
    using Generator              = std::mt19937_64;

    using byte_type              = std::uint8_t;
    using size_type              = std::size_t;

    using unsigned_type          = std::size_t;

    using MoveTailFunction       = unsigned_type (*)(unsigned_type);
    using Game2048DataAtFunction = unsigned_type& (Game2048::*)(size_type, size_type);

public:
    enum class Option : byte_type
    {
        null, up, down, left, right
    };

private:
    Generator generator_;

    unsigned_type* data_;

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
    size_type generate_insert_value(double probability_generate_2 = 0.9) noexcept;

    void random_insert_value() noexcept;

    unsigned_type& horizontal_access(size_type row_n, size_type i) noexcept;
    unsigned_type& vertical_access(size_type col_n, size_type i) noexcept;

    void join_tail(size_type i, size_type j,
                   MoveTailFunction move_tail,
                   Game2048DataAtFunction data_at) noexcept;

    void drop_zero_left(size_type n, Game2048DataAtFunction data_at) noexcept;
    void drop_zero_right(size_type n, Game2048DataAtFunction data_at) noexcept;

    static size_type move_inc(unsigned_type a) noexcept;
    static size_type move_dec(unsigned_type a) noexcept;

    void option_inc(Game2048DataAtFunction data_at) noexcept;
    void option_dec(Game2048DataAtFunction data_at) noexcept;
};

inline const std::size_t& Game2048::data(std::size_t i, std::size_t j) const noexcept
{
    return data_[i * shape_ + j];
}

inline const std::size_t* Game2048::data() const noexcept
{
    return data_;
}

inline std::size_t& Game2048::horizontal_access(std::size_t row_n, std::size_t i) noexcept
{
    return data_[row_n * shape_ + i];
}

inline std::size_t& Game2048::vertical_access(std::size_t col_n, std::size_t i) noexcept
{
    return data_[i * shape_ + col_n];
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
