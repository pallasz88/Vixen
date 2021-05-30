#ifndef VIXEN_RANDOM_HPP_INCLUDED
#define VIXEN_RANDOM_HPP_INCLUDED

#include <cstdint>

#if _MSC_VER >= 1910
#undef max
#undef min
#define NOMINMAX
#endif

namespace Vixen
{
/**
 *   Xn+1=(aXn + C) mod m
 **/
class PRNG
{
  private:
    struct Rand
    {
        uint64_t x = std::numeric_limits<uint64_t>::min();
        uint64_t m = std::numeric_limits<uint64_t>::max();
        uint64_t c = 2531011;
        uint64_t a = 6364136223846793005ULL;
    };

    Rand rand;

    constexpr uint64_t gen(uint64_t min, uint64_t max)
    {
        rand.x = (rand.a * rand.x + rand.c) % rand.m;
        return rand.x % max + min;
    }

  public:
    constexpr auto operator()(uint64_t min, uint64_t max)
    {
        return gen(min, max);
    }

    static constexpr uint64_t GenerateRandom(unsigned i = 0, uint64_t min = 0,
                                             uint64_t max = std::numeric_limits<uint64_t>::max())
    {
        PRNG gen;
        while (i > 0)
        {
            gen(min, max);
            --i;
        }
        return gen(min, max);
    }
};
} // namespace Vixen

#endif // VIXEN_RANDOM_HPP_INCLUDED