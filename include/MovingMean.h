#ifndef MOVING_MEAN_H_
#define MOVING_MEAN_H_

#include <array>

template <std::size_t WINDOW_SIZE>
class MovingMean {
public:
    MovingMean(void) : window{0,}, pos(0), sum(0) {
    }

    ~MovingMean(void) {
    }

    uint16_t update(uint16_t v) {
        pos %= WINDOW_SIZE;
        sum -= window[pos];
        window[pos] = v;
        sum += v;
        ++pos;
        return static_cast<uint16_t>(sum / WINDOW_SIZE);
    }

private:
    std::array<uint16_t, WINDOW_SIZE> window;
    size_t pos;
    uint16_t sum;
};

#endif