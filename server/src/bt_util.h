#ifndef INCLUDED_UTIL
#define INCLUDED_UTIL

#include <chrono>
#include <cstdlib>

namespace bt {

/**
 * @brief: Utility Clock class to calculate time passed
 */
class Clock {

public:
    typedef long long int Milliseconds;
    typedef std::chrono::steady_clock::time_point TimePoint;

    /**
     * @brief: Reset the last tick time of the clock
     */
    void reset();

    /**
     * @brief: Return the number of milliseconds since the
     * last Clock's tick. If this method is called the first time
     * it return the number of milliseconds since
     * the construct of the Clock object
     */
    Milliseconds tick();

private:

    TimePoint d_lastTickTime;

};

inline void Clock::reset() {
    d_lastTickTime = std::chrono::steady_clock::now();
}

inline Clock::Milliseconds Clock::tick() {
    TimePoint t = std::chrono::steady_clock::now();
    Milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t - d_lastTickTime).count();
    d_lastTickTime = t;
    return elapsed;
}

inline Clock::Milliseconds currentTimeInMilliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

/**
 * @brief: Generate random number between [0, 1]
 * @return
 */
inline double random() {
    return ((double) std::rand()) / RAND_MAX;
}

inline unsigned int randInt(unsigned int start, unsigned int end) {
    return (unsigned int) (start + (end - start) * random());
}


}

#endif