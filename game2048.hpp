#ifndef GAME2048_HPP
#define GAME2048_HPP

#include <cstdint> // uint8_t, size_t
#include <ctime> // time
#include <random> // mt19937_64
#include <utility> // swap
#include <algorithm> //sort

class Game2048
{
public:
    using byte_type     = std::uint8_t;
    using size_type     = std::size_t;
    using unsigned_type = std::size_t;
    using Generator     = std::mt19937_64;

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

    const unsigned_type& read_data(size_type i, size_type j) const noexcept { return data_[i * shape_ + j]; }
    const unsigned_type* data() const noexcept { return data_; }

    size_type length() const noexcept { return length_; }
    size_type shape() const noexcept { return shape_; }

    size_type score() const noexcept { return score_; }

    bool done() const noexcept { return done_; }

private:
    unsigned_type& data(size_type i, size_type j) noexcept { return data_[i * shape_ + j]; }

    size_type generate_insert_value(double probability_generate_2 = 0.75) noexcept;

    void random_insert_value() noexcept;

    void join_vertical(size_type i, size_type j, unsigned_type (*vertical_move)(unsigned_type)) noexcept;
    void join_horizontal(size_type i, size_type j, unsigned_type (*horizontal_move)(unsigned_type)) noexcept;

    void drop_vertical(size_type i, bool (*to_vertical_side)(unsigned_type, unsigned_type)) noexcept;
    void drop_horizontal(size_type j, bool (*to_horizontal_side)(unsigned_type, unsigned_type)) noexcept;

private:
    static size_type move_up(unsigned_type a) noexcept { return a + 1; }
    static size_type move_down(unsigned_type a) noexcept { return a - 1; }

    static bool to_up(unsigned_type a, unsigned_type b) noexcept { return b == 0 && a > b; }
    static bool to_down(unsigned_type a, unsigned_type b) noexcept { return a == 0 && a < b; }
};

#endif // GAME2048_HPP
