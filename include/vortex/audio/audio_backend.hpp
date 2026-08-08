#pragma once

namespace vortex::audio::backend
{
    // --- SOUND EFFECTS (Short, kept in RAM) ---
    struct VxSound {
        unsigned int id;
    };

    void initAudio();
    void closeAudio();

    VxSound loadSound(const char* filepath);
    void unloadSound(VxSound sound);
    
    void playSound(VxSound sound);
    void stopSound(VxSound sound);
    void setVolume(VxSound sound, float volume);

    // --- BACKGROUND MUSIC (Long, streamed from disk) ---
    struct VxMusic {
        unsigned int id;
    };

    VxMusic loadMusic(const char* filepath);
    void unloadMusic(VxMusic music);
    
    void playMusic(VxMusic music);
    void stopMusic(VxMusic music);
    void updateMusic(VxMusic music); // MUST be called every frame in your while() loop!
}
