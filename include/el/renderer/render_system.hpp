#pragma once

namespace el
{
    class ElRenderSystem
    {
    public:
        ElRenderSystem(void* reg);
        ~ElRenderSystem() = default;

        // Iterates through the ECS, grabs Transform + Render components, 
        // and submits them to the PencilRenderer.
        void update(); 

    private:
        void* m_registry; // Pointer to ElRegistry
    };
}
