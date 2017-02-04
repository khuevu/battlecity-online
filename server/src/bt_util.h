#ifndef INCLUDED_UTIL
#define INCLUDED_UTIL

#include <chrono>

namespace bt {

/**
 * @brief: Utility Timer class to calculate time passed
 */
class Timer {

    using TimePoint = std::chrono::system_clock::time_point;

public:

    /**
     * @brief: Start the Timer
     */
    void start(); 

    /**
     * @brief: Return the number of milliseconds since the 
     * Timer is started. If this method is called before the start
     * method is called, return the number of milliseconds since 
     * the construct of the Timer object
     */
    unsigned long stop(); 

private: 

    TimePoint d_startTime; 

};

inline void Timer::start() {
    d_startTime = std::chrono::system_clock::now(); 
}

inline unsigned long Timer::stop() {
    TimePoint end = std::chrono::system_clock::now(); 
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - d_startTime).count();
} 

inline unsigned long currentTimeInMilliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}
}

#endif 
