#include "vortex/renderer/renderer_backend.hpp"

// THIS IS THE ONLY FILE ALLOWED TO INCLUDE RAYLIB!
#include "raylib.h"

namespace vortex::renderer::backend
{
    void initWindow(int width, int height, const char *title)
    {
        SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);

        InitWindow(width, height, title);

        int monitor = GetCurrentMonitor();
        int screen_width = GetMonitorWidth(monitor);
        int screen_height = GetMonitorHeight(monitor);
        int refresh_rate = GetMonitorRefreshRate(monitor);

        SetWindowSize(screen_width, screen_height);
        SetWindowPosition(0, 0);

        ClearWindowState(FLAG_WINDOW_HIDDEN);

        SetWindowState(FLAG_FULLSCREEN_MODE);

        if (refresh_rate > 0)
            SetTargetFPS(refresh_rate);
        else
            SetTargetFPS(60);
    }

    void closeWindow()
    {
        CloseWindow();
    }

    bool shouldClose()
    {
        return WindowShouldClose();
    }

    void beginFrame()
    {
        BeginDrawing();
    }

    void executeQueue(const VxRenderQueue &queue)
    {
        const auto &commands = queue.getCommands();

        for (size_t i = 0; i < commands.size(); ++i)
        {
            const RenderCommand &cmd = commands[i];

            switch (cmd.type)
            {
            case ERenderCommandType::ClearScreen:
            {
                Color rayColor = {cmd.data.clear.color.r, cmd.data.clear.color.g, cmd.data.clear.color.b, cmd.data.clear.color.a};
                ClearBackground(rayColor);
                break;
            }
            case ERenderCommandType::DrawSprite:
            {
                if (cmd.data.sprite.texture == nullptr)
                    break;

                // Cast Vx types back to Raylib types (zero cost)
                Texture2D rayTex = {
                    cmd.data.sprite.texture->id,
                    cmd.data.sprite.texture->width,
                    cmd.data.sprite.texture->height,
                    cmd.data.sprite.texture->mipmaps,
                    cmd.data.sprite.texture->format};

                Rectangle raySource = {cmd.data.sprite.source.x, cmd.data.sprite.source.y, cmd.data.sprite.source.width, cmd.data.sprite.source.height};
                Rectangle rayDest = {cmd.data.sprite.dest.x, cmd.data.sprite.dest.y, cmd.data.sprite.dest.width, cmd.data.sprite.dest.height};
                Vector2 rayOrigin = {cmd.data.sprite.origin.x, cmd.data.sprite.origin.y};
                Color rayTint = {cmd.data.sprite.tint.r, cmd.data.sprite.tint.g, cmd.data.sprite.tint.b, cmd.data.sprite.tint.a};

                DrawTexturePro(rayTex, raySource, rayDest, rayOrigin, cmd.data.sprite.rotation, rayTint);
                break;
            }
            }
        }
    }

    void endFrame()
    {
        EndDrawing();
    }
}
