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
    using Game2048DataAtFunction     = unsigned_type& (Game2048::*)(size_type, size_type);
    using Game2048MoveOptionFunction = void (Game2048::*)(size_type&, unsigned_type*, Game2048DataAtFunction);
    using Game2048DropZeroFunction   = void (Game2048::*)(size_type, unsigned_type*, Game2048DataAtFunction);

public:
    enum class Option : byte_type
    {
        null, up, down, left, right
    };

private:
    Generator* generator_;
    unsigned_type* data_;

    size_type full_size_;
    size_type size_;

    size_type score_;

    bool done_;

public:
    Game2048(size_type side_size);
    Game2048(size_type side_size, unsigned_type* data, size_type score, bool done);

    ~Game2048();

    void reset() noexcept;

    void step(Option option) noexcept;

    const unsigned_type& data(size_type i, size_type j) const noexcept;
    const unsigned_type& data(size_type i) const noexcept;

    const unsigned_type* data() const noexcept { return data_; }

    size_type full_size() const noexcept { return full_size_; }
    size_type size() const noexcept { return size_; }

    size_type score() const noexcept { return score_; }

    bool done() const noexcept { return done_; }

private:
    unsigned_type& horizontal_access(size_type row_number, size_type i) noexcept;
    unsigned_type& vertical_access(size_type col_number, size_type i) noexcept;

    size_type generate_insert_value(double prob_gen_2 = 0.9) noexcept;

    void random_insert() noexcept;
    void update() noexcept;

    void join_tail(size_type i, size_type j,
                   MoveTailFunction move_tail,
                   Game2048DataAtFunction at) noexcept;

    void option_inc(Game2048DataAtFunction at) noexcept;
    void option_dec(Game2048DataAtFunction at) noexcept;

    void drop_zero_left(size_type n, Game2048DataAtFunction at) noexcept;
    void drop_zero_right(size_type n, Game2048DataAtFunction at) noexcept;

    bool has_inc_join(Game2048DataAtFunction at) noexcept;
    bool has_dec_join(Game2048DataAtFunction at) noexcept;

    bool check_inc(size_type n, Game2048DataAtFunction at) noexcept;
    bool check_dec(size_type n, Game2048DataAtFunction at) noexcept;

private:
    static size_type move_inc(unsigned_type a) noexcept { return ++a; }
    static size_type move_dec(unsigned_type a) noexcept { return --a; }
};

inline const std::size_t& Game2048::data(std::size_t i, std::size_t j) const noexcept
{
    return data_[i * size_ + j];
}

inline const std::size_t& Game2048::data(std::size_t i) const noexcept
{
    return data_[i];
}

inline std::size_t& Game2048::horizontal_access(std::size_t row_number, std::size_t i) noexcept
{
    return data_[row_number * size_ + i];
}

inline std::size_t& Game2048::vertical_access(std::size_t col_number, std::size_t i) noexcept
{
    return data_[i * size_ + col_number];
}

#endif // GAME2048_HPP
