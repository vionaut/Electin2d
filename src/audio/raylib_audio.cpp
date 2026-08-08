#include "vortex/audio/audio_backend.hpp"
#include "raylib.h"
// #include "miniaudio.h"

#include "vortex/core/containers/hash_map.hpp"
#include <unordered_map>

namespace vortex::audio::backend
{
    static containers::VxHashMap<unsigned int, Sound> g_sounds;
    static unsigned int g_nextSoundId = 1;

    static containers::VxHashMap<unsigned int, Music> g_music;
    static unsigned int g_nextMusicId = 1;

    void initAudio() {
        InitAudioDevice();
    }

    void closeAudio() {
        CloseAudioDevice();
    }

    // --- SOUND EFFECTS ---
    VxSound loadSound(const char* filepath) {
        Sound raySound = LoadSound(filepath);
        unsigned int id = g_nextSoundId++;
        g_sounds[id] = raySound;
        return { id };
    }

    void unloadSound(VxSound sound) {
        if (g_sounds.find(sound.id) != g_sounds.end()) {
            UnloadSound(g_sounds[sound.id]);
            g_sounds.erase(sound.id);
        }
    }

    void playSound(VxSound sound) {
        if (g_sounds.find(sound.id) != g_sounds.end())
            PlaySound(g_sounds[sound.id]);
    }

    void stopSound(VxSound sound) {
        if (g_sounds.find(sound.id) != g_sounds.end())
            StopSound(g_sounds[sound.id]);
    }

    void setVolume(VxSound sound, float volume) {
        if (g_sounds.find(sound.id) != g_sounds.end())
            SetSoundVolume(g_sounds[sound.id], volume);
    }

    // --- BACKGROUND MUSIC ---
    VxMusic loadMusic(const char* filepath) {
        Music rayMusic = LoadMusicStream(filepath);
        unsigned int id = g_nextMusicId++;
        g_music[id] = rayMusic;
        return { id };
    }

    void unloadMusic(VxMusic music) {
        if (g_music.find(music.id) != g_music.end()) {
            UnloadMusicStream(g_music[music.id]);
            g_music.erase(music.id);
        }
    }

    void playMusic(VxMusic music) {
        if (g_music.find(music.id) != g_music.end())
            PlayMusicStream(g_music[music.id]);
    }

    void stopMusic(VxMusic music) {
        if (g_music.find(music.id) != g_music.end())
            StopMusicStream(g_music[music.id]);
    }

    void updateMusic(VxMusic music) {
        if (g_music.find(music.id) != g_music.end())
            UpdateMusicStream(g_music[music.id]);
    }
}
