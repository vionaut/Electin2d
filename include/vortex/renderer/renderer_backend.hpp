#pragma once

#include "vortex/renderer/render_queue.hpp"

// Compile-Time Polymorphism Contract
// This file declares the interface. 
// ONLY the active renderer (e.g., raylib_renderer.cpp) will implement these.

namespace vortex::renderer::backend
{
    void initWindow(int width, int height, const char* title);
    void closeWindow();

    bool shouldClose();

    void beginFrame();
    void executeQueue(const VxRenderQueue& queue);
    void endFrame();
}
