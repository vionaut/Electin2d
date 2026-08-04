#pragma once

namespace vortex::utils
{
    class VxTime
    {
    public:
        // Initialize the time subsystem (call this at the very start of the Application)
        static void init();

        // Called exactly once per frame by the main loop to calculate delta times
        static void update();

        // --- Delta Time ---
        // The scaled delta time. Use this for all gameplay and physics logic.
        static double getDeltaTime();

        // The real, unscaled delta time. Use this for UI animations or networking.
        static double getUnscaledDeltaTime();

        // --- Total Time ---
        // Total time elapsed since init() *affected by time scale*. 
        static double getGameTime();

        // Total real-world time elapsed since init() in seconds.
        static double getRealTime();

        // --- Time Dilation ---
        // 1.0 is normal, 0.5 is slow motion, 0.0 is paused
        static void setTimeScale(double scale);
        static double getTimeScale();

    private:
        static double s_deltaTime;
        static double s_unscaledDeltaTime;
        static double s_gameTime;
        static double s_realTime;
        static double s_timeScale;
    };
}
