#include "el/core/assets/asset_manager.hpp"
#include "el/core/io/logger/el_logger.hpp"

#include "el/core/containers/hash_map.hpp"
#include "el/core/containers/string.hpp"
#include "el/core/containers/static_array.hpp"

namespace el
{
	struct ElAssetManager::Impl
	{
		// Maps string paths to internal Engine IDs
		ElHashMap<ElString, unsigned int> texturePathMap;
		ElHashMap<ElString, unsigned int> soundPathMap;
		ElHashMap<ElString, unsigned int> musicPathMap;

		// Maps internal Engine IDs to the actual Asset structs
		ElStaticArray<unsigned int, MAX_TEXTURES> textures_free_list;
		ElStaticArray<const ElString*, MAX_TEXTURES + 1> texture_paths;
		ElStaticArray<ElTexture, MAX_TEXTURES + 1> textures;

		ElStaticArray<unsigned int, MAX_SOUNDS> sounds_free_list;
		ElStaticArray<const ElString*, MAX_SOUNDS + 1> sound_paths;
		ElStaticArray<backend::ElSound, MAX_SOUNDS + 1> sounds;

		ElStaticArray<unsigned int, MAX_MUSICS> musics_free_list;
		ElStaticArray<const ElString*, MAX_MUSICS + 1> music_paths;
		ElStaticArray<backend::ElMusic, MAX_MUSICS + 1> musics;
	};

	void ElAssetManager::init()
	{
		EL_ASSERT(!m_impl, "Attempted Reinitialization of Asset Manager!...");

		m_impl = EL_NEW(Impl);

		for (size_t i = 0; i < MAX_TEXTURES; ++i)
		{
			m_impl->textures_free_list[i] = i + 1;
		}

		for (size_t i = 0; i < MAX_SOUNDS; ++i)
		{
			m_impl->sounds_free_list[i] = i + 1;
		}

		for (size_t i = 0; i < MAX_MUSICS; ++i)
		{
			m_impl->musics_free_list[i] = i + 1;
		}

		EL_LOG_INFO("Asset Manager Initialized");
	}

	void ElAssetManager::shutdown()
	{
		EL_ASSERT(m_impl, "Attempted shutdown of an uninitialized Asset Manager!...");

		for (unsigned int id = 1; id <= MAX_TEXTURES; ++id) {
			unloadTexture(id);
		}
		for (unsigned int id = 1; id <= MAX_SOUNDS; ++id) {
			unloadSound(id);
		}
		for (unsigned int id = 1; id <= MAX_MUSICS; ++id) {
			unloadMusic(id);
		}

		EL_DELETE(m_impl);
		EL_LOG_INFO("Asset Manager Destroyed");
	}

	unsigned int ElAssetManager::loadTexture(const char* filepath)
	{
		ElString path(filepath);

		if (m_impl->texturePathMap.find(path) != m_impl->texturePathMap.end())
		{
			return m_impl->texturePathMap[path];
		}

		unsigned int newId = m_impl->textures_free_list[0];
		m_impl->textures_free_list[0] = m_impl->textures_free_list[newId];
		m_impl->texturePathMap[path] = newId;

		ElTexture& new_texture = m_impl->textures[newId];
		const ElString*& new_path = m_impl->texture_paths[newId];

		auto path_map = m_impl->texturePathMap.find(path);
		new_path = &(path_map->first);
		new_texture = backend::loadTexture(filepath);

		EL_LOG_DEBUG("Loaded new texture: {}", path);
		return newId;
	}

	ElTexture* ElAssetManager::getTexture(unsigned int id)
	{
		if (m_impl->texture_paths[id])
		{
			return &m_impl->textures[id];
		}
		return nullptr;
	}

	unsigned int ElAssetManager::loadSound(const char* filepath)
	{
		ElString path(filepath);

		if (m_impl->soundPathMap.find(path) != m_impl->soundPathMap.end())
		{
			return m_impl->soundPathMap[path];
		}

		unsigned int newId = m_impl->sounds_free_list[0];
		m_impl->sounds_free_list[0] = m_impl->sounds_free_list[newId];
		m_impl->soundPathMap[path] = newId;

		backend::ElSound& new_sound = m_impl->sounds[newId];
		const ElString*& new_path = m_impl->sound_paths[newId];

		auto path_map = m_impl->soundPathMap.find(path);
		new_path = &(path_map->first);
		new_sound = backend::loadSound(filepath);

		EL_LOG_DEBUG("Loaded new Sound: {}", path);
		return newId;
	}

	backend::ElSound* ElAssetManager::getSound(unsigned int id)
	{
		if (m_impl->sound_paths[id])
		{
			return &m_impl->sounds[id];
		}
		return nullptr;
	}

	unsigned int ElAssetManager::loadMusic(const char* filepath)
	{
		ElString path(filepath);

		if (m_impl->musicPathMap.find(path) != m_impl->musicPathMap.end())
		{
			return m_impl->musicPathMap[path];
		}

		unsigned int newId = m_impl->musics_free_list[0];
		m_impl->musics_free_list[0] = m_impl->musics_free_list[newId];
		m_impl->musicPathMap[path] = newId;

		backend::ElMusic& new_music = m_impl->musics[newId];
		const ElString*& new_path = m_impl->music_paths[newId];

		auto path_map = m_impl->musicPathMap.find(path);
		new_path = &(path_map->first);
		new_music = backend::loadMusic(filepath);

		EL_LOG_DEBUG("Loaded new Music: {}", path);
		return newId;
	}

	backend::ElMusic* ElAssetManager::getMusic(unsigned int id)
	{
		if (m_impl->music_paths[id])
		{
			return &m_impl->musics[id];
		}
		return nullptr;
	}

	void ElAssetManager::unloadTexture(unsigned int id)
	{
		if (m_impl->texture_paths[id])
		{
			backend::unloadTexture(m_impl->textures[id]);
			m_impl->texturePathMap.erase(*(m_impl->texture_paths[id]));
			m_impl->texture_paths[id] = nullptr;
			m_impl->textures_free_list[id] = m_impl->textures_free_list[0];
			m_impl->textures_free_list[0] = id;
		}
	}

	void ElAssetManager::unloadSound(unsigned int id)
	{
		if (m_impl->sound_paths[id])
		{
			backend::unloadSound(m_impl->sounds[id]);
			m_impl->soundPathMap.erase(*(m_impl->sound_paths[id]));
			m_impl->sound_paths[id] = nullptr;
			m_impl->sounds_free_list[id] = m_impl->sounds_free_list[0];
			m_impl->sounds_free_list[0] = id;
		}
	}

	void ElAssetManager::unloadMusic(unsigned int id)
	{
		if (m_impl->music_paths[id])
		{
			backend::unloadMusic(m_impl->musics[id]);
			m_impl->musicPathMap.erase(*(m_impl->music_paths[id]));
			m_impl->music_paths[id] = nullptr;
			m_impl->musics_free_list[id] = m_impl->musics_free_list[0];
			m_impl->musics_free_list[0] = id;
		}
	}

	ElStaticArray<ElTexture, MAX_TEXTURES + 1>& ElAssetManager::getTexturesAll()
	{
		return m_impl->textures;
	}

	ElStaticArray<backend::ElSound, MAX_SOUNDS + 1>& ElAssetManager::getSoundsAll()
	{
		return m_impl->sounds;
	}

	ElStaticArray<backend::ElMusic, MAX_MUSICS + 1>& ElAssetManager::getMusicsAll()
	{
		return m_impl->musics;
	}
}