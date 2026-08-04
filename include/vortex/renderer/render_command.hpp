#pragma once

#include "vortex/renderer/vortex_graphics.hpp"
#include "vortex/core/math/vortex_math.hpp"
#include <cstdint>

namespace vortex::renderer
{
    enum class ERenderCommandType : uint8_t
    {
        ClearScreen,
        DrawSprite
    };

    struct RenderCommand
    {
        ERenderCommandType type;
        
        // Depth/Z-index for sorting the queue later
        float z_index;

        // We use a union to save memory. A command is only one of these at a time.
        union Payload {
            struct { 
                VxColor color; 
            } clear;

            struct {
                const VxTexture* texture; // Pointer prevents copying massive structs
                math::VxRect source;
                math::VxRect dest;
                math::VxVec2 origin;
                float rotation;
                VxColor tint;
            } sprite;
        } data;
    };
}
