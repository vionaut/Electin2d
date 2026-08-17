#pragma once

namespace el::backend
{
    // --- SOUND EFFECTS (Short, kept in RAM) ---
    struct ElSound {
        unsigned int id;
    };

    void initAudio();
    void closeAudio();

    ElSound loadSound(const char* filepath);
    void unloadSound(ElSound sound);
    
    void playSound(ElSound sound);
    void stopSound(ElSound sound);
    void setVolume(ElSound sound, float volume);

    // --- BACKGROUND MUSIC (Long, streamed from disk) ---
    struct ElMusic {
        unsigned int id;
    };

    ElMusic loadMusic(const char* filepath);
    void unloadMusic(ElMusic music);
    
    void playMusic(ElMusic music);
    void stopMusic(ElMusic music);
    void updateMusic(ElMusic music); // MUST be called every frame in your while() loop!
}
