#pragma once

#include "vortex/renderer/vortex_graphics.hpp"
#include "vortex/core/assets/asset_manager.hpp"
#include "vortex/renderer/command_buffer.hpp"
#include "vortex/renderer/renderer_backend.hpp"
#include "vortex/core/utilities/memory.hpp"

namespace vortex::renderer
{
	class VxPencilRenderer
	{
	public:


	private:
		utils::VxUniquePtr<renderer::VxCommandBuffer> m_commandBuffer;
		utils::VxUniquePtr<core::VxAssetManager> m_assetManager;
	};
}