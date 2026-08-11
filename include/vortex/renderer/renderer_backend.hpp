#pragma once

#include "vortex/renderer/command_buffer.hpp"
#include "vortex/renderer/vortex_graphics.hpp"
#include "vortex/core/containers/static_array.hpp"

// Compile-Time Polymorphism Contract
// This file declares the interface.
// ONLY the active renderer (e.g., raylib_renderer.cpp) will implement these.

namespace vortex::renderer::backend
{
    enum class EResolutionMode
    {
        Expand,
        PillarBox,
        Stretch
    };

    void setResolutionMode(EResolutionMode mode);
    void setCanvasResolution(int width, int height);
    void setFPS(int fps);
    void setVsync(bool set);
    void setFullScreen(bool set);

    void initWindow(int width, int height, const char *title);
    void closeWindow();

    bool shouldClose();

    VxTexture loadTexture(const char *file_path);
    void unloadTexture(VxTexture texture);

    void beginFrame();

    /*
    * @brief executes the render commands every frame
    @param buffer refrence to the command buffer
    @param textures reference to the array containing texture ids
    @param camera pretty self explanatory, but yeah a reference to the current camera
    */

    void executeQueue(const VxCommandBuffer &buffer, containers::VxStaticArray<VxTexture, config::MAX_TEXTURES + 1>& textures, const VxCamera2d& camera);
    void endFrame();
}