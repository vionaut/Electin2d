#include "el/core/utilities/el_time.hpp"
#include <chrono>

namespace el
{
    static std::chrono::time_point<std::chrono::high_resolution_clock> s_startTime;
    static std::chrono::time_point<std::chrono::high_resolution_clock> s_lastTime;

    double ElTime::s_deltaTime = 0.0;
    double ElTime::s_unscaledDeltaTime = 0.0;
    double ElTime::s_gameTime = 0.0;
    double ElTime::s_realTime = 0.0;
    double ElTime::s_timeScale = 1.0;

    void ElTime::init()
    {
        s_startTime = std::chrono::high_resolution_clock::now();
        s_lastTime = s_startTime;
    }

    void ElTime::update()
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dt_duration = now - s_lastTime;
        s_lastTime = now;

        s_unscaledDeltaTime = dt_duration.count();

        s_deltaTime = s_unscaledDeltaTime * s_timeScale;

        s_realTime += s_unscaledDeltaTime;
        s_gameTime += s_deltaTime;
    }

    double ElTime::getDeltaTime() { return s_deltaTime; }
    double ElTime::getUnscaledDeltaTime() { return s_unscaledDeltaTime; }
    double ElTime::getGameTime() { return s_gameTime; }
    double ElTime::getRealTime() { return s_realTime; }

    void ElTime::setTimeScale(double scale) { s_timeScale = scale; }
    double ElTime::getTimeScale() { return s_timeScale; }
}
