#pragma once

#include "vortex/renderer/renderer_backend.hpp"
#include "vortex/audio/audio_backend.hpp"
#include "vortex/core/config/vortex_config.hpp"

namespace vortex::containers 
{
	template <typename T, size_t size>
	class VxStaticArray;
}

namespace vortex::core
{
	class VxAssetManager
	{
	public:

		static void init();
		static void shutdown();

		static unsigned int loadTexture(const char* filepath);
		static renderer::VxTexture* getTexture(unsigned int id);

		static unsigned int loadSound(const char* filepath);
		static audio::backend::VxSound* getSound(unsigned int id);

		static unsigned int loadMusic(const char* filepath);
		static audio::backend::VxMusic* getMusic(unsigned int id);

		static void unloadTexture(unsigned int id);
		static void unloadSound(unsigned int id);
		static void unloadMusic(unsigned int id);

		static containers::VxStaticArray<renderer::VxTexture, config::MAX_TEXTURES + 1>& getTexturesAll();

		static containers::VxStaticArray<audio::backend::VxSound, config::MAX_SOUNDS + 1>& getSoundsAll();

		static containers::VxStaticArray<audio::backend::VxMusic, config::MAX_MUSICS + 1>& getMusicsAll();

	private:
		struct Impl;
		static Impl* m_impl = nullptr;
	};
}