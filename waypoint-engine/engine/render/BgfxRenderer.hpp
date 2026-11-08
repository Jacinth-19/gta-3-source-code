#pragma once

#include <filesystem>
#include <string>

#include <bgfx/bgfx.h>

#include "core/Types.hpp"
#include "platform/SdlWindow.hpp"

namespace waypoint::render {

class BgfxRenderer {
public:
    bool Init(int width, int height, const platform::NativeWindowHandles& handles,
              const std::filesystem::path& shaderDirectory = {});
    void Shutdown();
    void Resize(int width, int height);
    void BeginFrame();
    void SetCamera(const Mat4& view, const Mat4& projection);
    void DrawCube(const Mat4& model);
    void DrawGrid(int halfCellCount = 20, float cellSize = 1.0f);
    void EndFrame();
    std::string BackendName() const;
    bool HasShaders() const { return bgfx::isValid(program_); }

private:
    bool LoadShaders(const std::filesystem::path& shaderDirectory);
    void CreateGeometry();
    void DestroyGeometry();

    bool initialized_ = false;
    int width_ = 0;
    int height_ = 0;
    Mat4 view_{1.0f};
    Mat4 projection_{1.0f};

    bgfx::VertexLayout layout_{};
    bgfx::ProgramHandle program_ = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle cubeVertices_ = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle cubeIndices_ = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle gridVertices_ = BGFX_INVALID_HANDLE;
    u32 gridVertexCount_ = 0;
};

} // namespace waypoint::render
