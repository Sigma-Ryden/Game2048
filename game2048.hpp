#ifndef GAME2048_HPP
#define GAME2048_HPP

#include <cstdint> // uint8_t, size_t
#include <ctime> // time
#include <random> // mt19937_64
#include <utility> // swap

class Game2048
{
public:
    using Generator       = std::mt19937_64;

    using byte_type       = std::uint8_t;
    using size_type       = std::size_t;

    using tile_value_type = std::size_t;

    using pointer         = tile_value_type*;
    using const_pointer   = const tile_value_type*;

    using reference       = tile_value_type&;
    using const_reference = const tile_value_type&;

private:
    template <typename Ret, typename... Args>
    using Func = Ret (*)(Args...);

    template <typename Ret, typename... Args>
    using MFunc = Ret (Game2048::*)(Args...);

    using FuncMoveTile    = Func<size_type, size_type>;
    using MFuncDataAt     = MFunc<reference, size_type, size_type>;

public:
    enum class Option : byte_type
    {
        null, up, down, left, right
    };

private:
    Generator* generator_;
    pointer data_;

    size_type full_size_;
    size_type size_;

    size_type score_;

    bool done_;

public:
    Game2048(size_type side_size);
    Game2048(size_type side_size, const_pointer data, size_type score, bool done);

    ~Game2048();

    void reset() noexcept;

    void step(Option option) noexcept;

    const_reference data(size_type i, size_type j) const noexcept;
    const_reference data(size_type i) const noexcept;

    const_pointer data() const noexcept { return data_; }

    size_type full_size() const noexcept { return full_size_; }
    size_type size() const noexcept { return size_; }

    size_type score() const noexcept { return score_; }

    bool done() const noexcept { return done_; }

private:
    reference horizontal_access(size_type row_number, size_type i) noexcept;
    reference vertical_access(size_type col_number, size_type i) noexcept;

    tile_value_type generate_insert_value(double gen2 = 0.9) noexcept;

    void random_insert() noexcept;
    void update() noexcept;

    void join_tail(size_type i, size_type j,
                   FuncMoveTile move_tale,
                   MFuncDataAt at) noexcept;

    void option_inc(MFuncDataAt at) noexcept;
    void option_dec(MFuncDataAt at) noexcept;

    void drop_zero_left(size_type n, MFuncDataAt at) noexcept;
    void drop_zero_right(size_type n, MFuncDataAt at) noexcept;

    bool has_inc_join(MFuncDataAt at) noexcept;
    bool has_dec_join(MFuncDataAt at) noexcept;

    bool check_inc(size_type n, MFuncDataAt at) noexcept;
    bool check_dec(size_type n, MFuncDataAt at) noexcept;

private:
    static size_type move_inc(size_type a) noexcept { return ++a; }
    static size_type move_dec(size_type a) noexcept { return --a; }
};

inline typename Game2048::const_reference Game2048::data(
    size_type i, size_type j) const noexcept
{
    return data_[i * size_ + j];
}

inline typename Game2048::const_reference Game2048::data(
    size_type i) const noexcept
{
    return data_[i];
}

inline typename Game2048::reference Game2048::horizontal_access(
    size_type row_number, size_type i) noexcept
{
    return data_[row_number * size_ + i];
}

inline typename Game2048::reference Game2048::vertical_access(
    size_type col_number, size_type i) noexcept
{
    return data_[i * size_ + col_number];
}

#endif // GAME2048_HPP
