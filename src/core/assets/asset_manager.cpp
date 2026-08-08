#include "vortex/core/assets/asset_manager.hpp"
#include "vortex/core/io/logger/vortex_logger.hpp"
#include "vortex/core/config/vortex_config.hpp"

#include "vortex/core/containers/hash_map.hpp"
#include "vortex/core/containers/string.hpp"
#include "vortex/core/containers/static_array.hpp"

namespace vortex::core
{
	struct VxAssetManager::Impl
	{
		// Maps string paths to internal Engine IDs
		containers::VxHashMap<containers::VxString, unsigned int> texturePathMap;
		containers::VxHashMap<containers::VxString, unsigned int> soundPathMap;
		containers::VxHashMap<containers::VxString, unsigned int> musicPathMap;

		// Maps internal Engine IDs to the actual Asset structs
		containers::VxStaticArray<unsigned int, config::MAX_TEXTURES> textures_free_list;
		containers::VxStaticArray<const containers::VxString*, config::MAX_TEXTURES + 1> texture_paths;
		containers::VxStaticArray<renderer::VxTexture, config::MAX_TEXTURES + 1> textures;

		containers::VxStaticArray<unsigned int, config::MAX_SOUNDS> sounds_free_list;
		containers::VxStaticArray<const containers::VxString*, config::MAX_SOUNDS + 1> sound_paths;
		containers::VxStaticArray<audio::backend::VxSound, config::MAX_SOUNDS + 1> sounds;

		containers::VxStaticArray<unsigned int, config::MAX_MUSICS> musics_free_list;
		containers::VxStaticArray<const containers::VxString*, config::MAX_MUSICS + 1> music_paths;
		containers::VxStaticArray<audio::backend::VxMusic, config::MAX_MUSICS + 1> musics;
	};

	VxAssetManager::VxAssetManager()
		: m_nextTextureId(1), m_nextSoundId(1), m_nextMusicId(1)
	{
		m_impl = utils::vxMakeUnique<Impl>();

		for (size_t i = 0; i < config::MAX_TEXTURES; ++i)
		{
			m_impl->textures_free_list[i] = i + 1;
		}

		for (size_t i = 0; i < config::MAX_SOUNDS; ++i)
		{
			m_impl->sounds_free_list[i] = i + 1;
		}

		for (size_t i = 0; i < config::MAX_MUSICS; ++i)
		{
			m_impl->musics_free_list[i] = i + 1;
		}

		VX_LOG_DEBUG("Asset Manager Initialized");
	}

	VxAssetManager::~VxAssetManager()
	{
		for (unsigned int id = 1; id <= config::MAX_TEXTURES; ++id) {
			this->unloadTexture(id);
		}
		for (unsigned int id = 1; id <= config::MAX_SOUNDS; ++id) {
			this->unloadSound(id);
		}
		for (unsigned int id = 1; id <= config::MAX_MUSICS; ++id) {
			this->unloadMusic(id);
		}

		VX_LOG_DEBUG("Asset Manager Destroyed");
	}

	unsigned int VxAssetManager::loadTexture(const char* filepath)
	{
		containers::VxString path(filepath);

		if (m_impl->texturePathMap.find(path) != m_impl->texturePathMap.end())
		{
			return m_impl->texturePathMap[path];
		}

		unsigned int newId = m_impl->textures_free_list[0];
		m_impl->textures_free_list[0] = m_impl->textures_free_list[newId];
		m_impl->texturePathMap[path] = newId;

		renderer::VxTexture& new_texture = m_impl->textures[newId];
		const containers::VxString*& new_path = m_impl->texture_paths[newId];

		auto path_map = m_impl->texturePathMap.find(path);
		new_path = &(path_map->first);
		new_texture = renderer::backend::loadTexture(filepath);

		VX_LOG_DEBUG("Loaded new texture: {}", path);
		return newId;
	}

	renderer::VxTexture* VxAssetManager::getTexture(unsigned int id)
	{
		if (m_impl->texture_paths[id])
		{
			return &m_impl->textures[id];
		}
		return nullptr;
	}

	unsigned int VxAssetManager::loadSound(const char* filepath)
	{
		containers::VxString path(filepath);

		if (m_impl->soundPathMap.find(path) != m_impl->soundPathMap.end())
		{
			return m_impl->soundPathMap[path];
		}

		unsigned int newId = m_impl->sounds_free_list[0];
		m_impl->sounds_free_list[0] = m_impl->sounds_free_list[newId];
		m_impl->soundPathMap[path] = newId;

		audio::backend::VxSound& new_sound = m_impl->sounds[newId];
		const containers::VxString*& new_path = m_impl->sound_paths[newId];

		auto path_map = m_impl->soundPathMap.find(path);
		new_path = &(path_map->first);
		new_sound = audio::backend::loadSound(filepath);

		VX_LOG_DEBUG("Loaded new Sound: {}", path);
		return newId;
	}

	audio::backend::VxSound* VxAssetManager::getSound(unsigned int id)
	{
		if (m_impl->sound_paths[id])
		{
			return &m_impl->sounds[id];
		}
		return nullptr;
	}

	unsigned int VxAssetManager::loadMusic(const char* filepath)
	{
		containers::VxString path(filepath);

		if (m_impl->musicPathMap.find(path) != m_impl->musicPathMap.end())
		{
			return m_impl->musicPathMap[path];
		}

		unsigned int newId = m_impl->musics_free_list[0];
		m_impl->musics_free_list[0] = m_impl->musics_free_list[newId];
		m_impl->musicPathMap[path] = newId;

		audio::backend::VxMusic& new_music = m_impl->musics[newId];
		const containers::VxString*& new_path = m_impl->music_paths[newId];

		auto path_map = m_impl->musicPathMap.find(path);
		new_path = &(path_map->first);
		new_music = audio::backend::loadMusic(filepath);

		VX_LOG_DEBUG("Loaded new Music: {}", path);
		return newId;
	}

	audio::backend::VxMusic* VxAssetManager::getMusic(unsigned int id)
	{
		if (m_impl->music_paths[id])
		{
			return &m_impl->musics[id];
		}
		return nullptr;
	}

	void VxAssetManager::unloadTexture(unsigned int id)
	{
		if (m_impl->texture_paths[id])
		{
			renderer::backend::unloadTexture(m_impl->textures[id]);
			m_impl->texturePathMap.erase(*(m_impl->texture_paths[id]));
			m_impl->texture_paths[id] = nullptr;
			m_impl->textures_free_list[id] = m_impl->textures_free_list[0];
			m_impl->textures_free_list[0] = id;
		}
	}

	void VxAssetManager::unloadSound(unsigned int id)
	{
		if (m_impl->sound_paths[id])
		{
			audio::backend::unloadSound(m_impl->sounds[id]);
			m_impl->soundPathMap.erase(*(m_impl->sound_paths[id]));
			m_impl->sound_paths[id] = nullptr;
			m_impl->sounds_free_list[id] = m_impl->sounds_free_list[0];
			m_impl->sounds_free_list[0] = id;
		}
	}

	void VxAssetManager::unloadMusic(unsigned int id)
	{
		if (m_impl->music_paths[id])
		{
			audio::backend::unloadMusic(m_impl->musics[id]);
			m_impl->musicPathMap.erase(*(m_impl->music_paths[id]));
			m_impl->music_paths[id] = nullptr;
			m_impl->musics_free_list[id] = m_impl->musics_free_list[0];
			m_impl->musics_free_list[0] = id;
		}
	}
}