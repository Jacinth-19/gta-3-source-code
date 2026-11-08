#include "render/BgfxRenderer.hpp"

#include <bgfx/bgfx.h>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "core/FileSystem.hpp"
#include "core/Log.hpp"

namespace waypoint::render {
namespace {

struct PosColorVertex {
    float x, y, z;
    uint32_t abgr;
};

// clang-format off
static const PosColorVertex s_cubeVertices[] = {
    {-1.0f,  1.0f,  1.0f, 0xff000000},
    { 1.0f,  1.0f,  1.0f, 0xff0000ff},
    {-1.0f, -1.0f,  1.0f, 0xff00ff00},
    { 1.0f, -1.0f,  1.0f, 0xff00ffff},
    {-1.0f,  1.0f, -1.0f, 0xffff0000},
    { 1.0f,  1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00},
    { 1.0f, -1.0f, -1.0f, 0xffffffff},
};

static const uint16_t s_cubeIndices[] = {
    0, 1, 2, 1, 3, 2,
    4, 6, 5, 5, 6, 7,
    0, 2, 4, 4, 2, 6,
    1, 5, 3, 5, 7, 3,
    0, 4, 1, 4, 5, 1,
    2, 3, 6, 6, 3, 7,
};
// clang-format on

bgfx::RendererType::Enum SelectRendererType() {
    const char* preference = std::getenv("WAYPOINT_RENDERER");
    if (!preference) {
        return bgfx::RendererType::Count;
    }
    const std::string value(preference);
    if (value == "vulkan") return bgfx::RendererType::Vulkan;
    if (value == "opengl") return bgfx::RendererType::OpenGL;
    if (value == "opengles") return bgfx::RendererType::OpenGLES;
    if (value == "d3d11") return bgfx::RendererType::Direct3D11;
    if (value == "d3d12") return bgfx::RendererType::Direct3D12;
    if (value == "metal") return bgfx::RendererType::Metal;
    return bgfx::RendererType::Count;
}

const char* ShaderSubdirectory(bgfx::RendererType::Enum type) {
    switch (type) {
    case bgfx::RendererType::Direct3D11: return "dxbc";
    case bgfx::RendererType::Direct3D12: return "dxil";
    case bgfx::RendererType::Metal: return "metal";
    case bgfx::RendererType::OpenGL: return "glsl";
    case bgfx::RendererType::OpenGLES: return "essl";
    case bgfx::RendererType::Vulkan: return "spirv";
    default: return "spirv";
    }
}

bgfx::ShaderHandle LoadShader(const std::filesystem::path& path) {
    const auto bytes = fs::ReadTextFile(path);
    if (!bytes || bytes->empty()) {
        log::Warn("Shader not found: " + path.string());
        return BGFX_INVALID_HANDLE;
    }
    const bgfx::Memory* memory = bgfx::copy(bytes->data(), static_cast<uint32_t>(bytes->size()));
    bgfx::ShaderHandle handle = bgfx::createShader(memory);
    if (!bgfx::isValid(handle)) {
        log::Error("Failed to create shader: " + path.string());
    }
    return handle;
}

} // namespace

bool BgfxRenderer::Init(int width, int height, const platform::NativeWindowHandles& handles,
                        const std::filesystem::path& shaderDirectory) {
    bgfx::Init init;
    init.type = SelectRendererType();
    init.vendorId = BGFX_PCI_ID_NONE;
    init.fallback = true;
    init.reset = BGFX_RESET_VSYNC;
    init.swapChain.nwh = handles.window;
    init.swapChain.ndt = handles.display;
    init.swapChain.width = static_cast<uint32_t>(width);
    init.swapChain.height = static_cast<uint32_t>(height);
    init.platformData.type = handles.wayland ? bgfx::NativeWindowHandleType::Wayland
                                             : bgfx::NativeWindowHandleType::Default;

    if (!bgfx::init(init)) {
        log::Error("bgfx::init failed");
        return false;
    }
    width_ = width;
    height_ = height;
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x101018ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));

    layout_.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();

    CreateGeometry();
    if (!shaderDirectory.empty()) {
        LoadShaders(shaderDirectory);
    } else {
        log::Warn("No shader directory configured; rendering debug text only.");
    }

    initialized_ = true;
    log::Info(std::string("bgfx backend: ") + BackendName());
    return true;
}

bool BgfxRenderer::LoadShaders(const std::filesystem::path& shaderDirectory) {
    const auto directory = shaderDirectory / ShaderSubdirectory(bgfx::getRendererType());
    bgfx::ShaderHandle vertexShader = LoadShader(directory / "vs_basic.bin");
    bgfx::ShaderHandle fragmentShader = LoadShader(directory / "fs_basic.bin");
    if (!bgfx::isValid(vertexShader) || !bgfx::isValid(fragmentShader)) {
        if (bgfx::isValid(vertexShader)) bgfx::destroy(vertexShader);
        if (bgfx::isValid(fragmentShader)) bgfx::destroy(fragmentShader);
        return false;
    }
    program_ = bgfx::createProgram(vertexShader, fragmentShader, true);
    if (!bgfx::isValid(program_)) {
        log::Error("Failed to create basic rendering program");
        return false;
    }
    return true;
}

void BgfxRenderer::CreateGeometry() {
    cubeVertices_ = bgfx::createVertexBuffer(
        bgfx::copy(s_cubeVertices, sizeof(s_cubeVertices)), layout_);
    cubeIndices_ = bgfx::createIndexBuffer(
        bgfx::copy(s_cubeIndices, sizeof(s_cubeIndices)));
}

void BgfxRenderer::DestroyGeometry() {
    if (bgfx::isValid(cubeVertices_)) bgfx::destroy(cubeVertices_);
    if (bgfx::isValid(cubeIndices_)) bgfx::destroy(cubeIndices_);
    if (bgfx::isValid(gridVertices_)) bgfx::destroy(gridVertices_);
    if (bgfx::isValid(program_)) bgfx::destroy(program_);
    cubeVertices_ = BGFX_INVALID_HANDLE;
    cubeIndices_ = BGFX_INVALID_HANDLE;
    gridVertices_ = BGFX_INVALID_HANDLE;
    program_ = BGFX_INVALID_HANDLE;
}

void BgfxRenderer::Shutdown() {
    if (initialized_) {
        DestroyGeometry();
        bgfx::shutdown();
        initialized_ = false;
    }
}

void BgfxRenderer::Resize(int width, int height) {
    if (!initialized_ || width <= 0 || height <= 0 || (width == width_ && height == height_)) {
        return;
    }
    width_ = width;
    height_ = height;
    bgfx::SwapChain swapChain;
    swapChain.width = static_cast<uint32_t>(width);
    swapChain.height = static_cast<uint32_t>(height);
    bgfx::reset(BGFX_RESET_VSYNC, &swapChain);
    bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));
}

void BgfxRenderer::BeginFrame() {
    if (!initialized_) {
        return;
    }
    bgfx::setViewTransform(0, &view_, &projection_);
    bgfx::touch(0);
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 0, 0x0f, "Waypoint Engine - bgfx backend: %s", BackendName().c_str());
    if (!HasShaders()) {
        bgfx::dbgTextPrintf(0, 1, 0x0e, "Shaders missing: build with WAYPOINT_BUILD_BGFX_TOOLS=ON");
    }
}

void BgfxRenderer::SetCamera(const Mat4& view, const Mat4& projection) {
    view_ = view;
    projection_ = projection;
}

void BgfxRenderer::DrawCube(const Mat4& model) {
    if (!initialized_ || !HasShaders()) {
        return;
    }
    bgfx::setTransform(&model);
    bgfx::setVertexBuffer(0, cubeVertices_);
    bgfx::setIndexBuffer(cubeIndices_);
    bgfx::setState(BGFX_STATE_DEFAULT);
    bgfx::submit(0, program_);
}

void BgfxRenderer::DrawGrid(int halfCellCount, float cellSize) {
    if (!initialized_ || !HasShaders() || halfCellCount <= 0 || cellSize <= 0.0f) {
        return;
    }

    const int lineCount = (halfCellCount * 2 + 1) * 2;
    const uint32_t vertexCount = static_cast<uint32_t>(lineCount * 2);
    const uint32_t indexCount = vertexCount;
    if (bgfx::getAvailTransientVertexBuffer(vertexCount, layout_) < vertexCount) {
        return;
    }

    bgfx::TransientVertexBuffer vertexBuffer;
    bgfx::TransientIndexBuffer indexBuffer;
    bgfx::allocTransientVertexBuffer(&vertexBuffer, vertexCount, layout_);
    bgfx::allocTransientIndexBuffer(&indexBuffer, indexCount);

    auto* vertices = reinterpret_cast<PosColorVertex*>(vertexBuffer.data);
    auto* indices = reinterpret_cast<uint16_t*>(indexBuffer.data);
    const float extent = static_cast<float>(halfCellCount) * cellSize;
    uint32_t vertex = 0;
    for (int i = -halfCellCount; i <= halfCellCount; ++i) {
        const float offset = static_cast<float>(i) * cellSize;
        const uint32_t axisColor = i == 0 ? 0xff606060 : 0xff303030;
        vertices[vertex++] = {-extent, 0.0f, offset, axisColor};
        vertices[vertex++] = {extent, 0.0f, offset, axisColor};
        vertices[vertex++] = {offset, 0.0f, -extent, axisColor};
        vertices[vertex++] = {offset, 0.0f, extent, axisColor};
    }
    for (uint32_t i = 0; i < indexCount; ++i) {
        indices[i] = static_cast<uint16_t>(i);
    }

    const Mat4 identity{1.0f};
    bgfx::setTransform(&identity);
    bgfx::setVertexBuffer(0, &vertexBuffer);
    bgfx::setIndexBuffer(&indexBuffer);
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_PT_LINES | BGFX_STATE_DEPTH_TEST_LESS);
    bgfx::submit(0, program_);
}

void BgfxRenderer::EndFrame() {
    if (initialized_) {
        bgfx::frame();
    }
}

std::string BgfxRenderer::BackendName() const {
    return bgfx::getRendererName(bgfx::getRendererType());
}
} // namespace waypoint::render
