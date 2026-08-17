#pragma once

#include "el/renderer/renderer_backend.hpp"
#include "el/audio/audio_backend.hpp"
#include "el/core/config/el_config.hpp"

namespace el 
{
	template <typename T, size_t size>
	class ElStaticArray;
}

namespace el
{
	class ElAssetManager
	{
	public:

		static void init();
		static void shutdown();

		static unsigned int loadTexture(const char* filepath);
		static ElTexture* getTexture(unsigned int id);

		static unsigned int loadSound(const char* filepath);
		static backend::ElSound* getSound(unsigned int id);

		static unsigned int loadMusic(const char* filepath);
		static backend::ElMusic* getMusic(unsigned int id);

		static void unloadTexture(unsigned int id);
		static void unloadSound(unsigned int id);
		static void unloadMusic(unsigned int id);

		static ElStaticArray<ElTexture, MAX_TEXTURES + 1>& getTexturesAll();

		static ElStaticArray<backend::ElSound, MAX_SOUNDS + 1>& getSoundsAll();

		static ElStaticArray<backend::ElMusic, MAX_MUSICS + 1>& getMusicsAll();

	private:
		struct Impl;
		inline static Impl* m_impl = nullptr;
	};
}