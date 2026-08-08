#pragma once

#include "vortex/core/config/vortex_config.hpp"
#include "vortex/core/containers/vortex_array.hpp"
#include "vortex/renderer/vortex_graphics.hpp"
#include "vortex/core/io/logger/vortex_logger.hpp"
#include "vortex/core/utilities/algorithm.hpp"

#include <cstdint>

namespace vortex::renderer
{
	class VxCommandBuffer
	{
	public:
		VxCommandBuffer(size_t size = 2 * 1024 * 1024)
			: m_size(size), m_cursor(0)
		{
			m_buffer = VX_NEW_S(uint8_t, m_size);
			m_keys.reserve(config::MAX_RENDER_COMMANDS);
		}

		~VxCommandBuffer()
		{
			VX_DELETE_S(m_buffer);
		}

		template <typename T>
		void submit(ERenderCommandType type, unsigned int z_index, unsigned int texture_id, const T& cmd)
		{
			VX_ASSERT(m_keys.size() < config::MAX_RENDER_COMMANDS, "Max render commands reached!");
			m_keys.push_back(generateCommandKey(z_index, texture_id, m_cursor));

			VX_ASSERT(m_size - m_cursor >= sizeof(ERenderCommandType) + sizeof(T), "Render Command Buffer exauhsted!");
			utils::memcpy(m_buffer + m_cursor, &type, sizeof(ERenderCommandType));
			m_cursor += sizeof(ERenderCommandType);

			utils::memcpy(m_buffer + m_cursor, &cmd, sizeof(T));
			m_cursor += sizeof(T);
		}

		void sort()
		{
			utils::sort(m_keys.begin(), m_keys.end());
		}

		void clear()
		{
			m_cursor = 0;
			m_keys.clear();
		}

		const containers::VxArray<uint64_t>& getKeys() const { return m_keys; }
		const uint8_t* getBuffer() const { return m_buffer; }

	private:
		uint64_t generateCommandKey(unsigned int z_index, unsigned int texture_id, unsigned int offset)
		{
			uint64_t key = 0;

			key |= (static_cast<uint64_t>(z_index) << 48);
			key |= (static_cast<uint64_t>(texture_id) << 32);
			key |= (static_cast<uint64_t>(offset));

			return key;
		}

		uint8_t* m_buffer;
		containers::VxArray<uint64_t> m_keys;
		size_t m_size;
		size_t m_cursor;
	};
}