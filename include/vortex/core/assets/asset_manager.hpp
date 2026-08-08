#pragma once

#include "vortex/renderer/renderer_backend.hpp"
#include "vortex/audio/audio_backend.hpp"

#include "vortex/core/utilities/memory.hpp"

namespace vortex::core
{
	class VxAssetManager
	{
	public:
		VxAssetManager();
		~VxAssetManager();

		unsigned int loadTexture(const char* filepath);
		renderer::VxTexture* getTexture(unsigned int id);

		unsigned int loadSound(const char* filepath);
		audio::backend::VxSound* getSound(unsigned int id);

		unsigned int loadMusic(const char* filepath);
		audio::backend::VxMusic* getMusic(unsigned int id);

		void unloadTexture(unsigned int id);
		void unloadSound(unsigned int id);
		void unloadMusic(unsigned int id);

	private:
		struct Impl;
		utils::VxUniquePtr<Impl> m_impl;
	};
}