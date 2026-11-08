# Compiles the engine's beginner-friendly basic shaders with bgfx shaderc.
#
# Usage:
#   waypoint_compile_basic_shaders(<target> <output-dir>)
#
# The output directory receives one folder per bgfx shader profile:
#   spirv/ glsl/ essl/ metal/ dxbc/ dxil/
# Each folder contains vs_basic.bin and fs_basic.bin.

function(waypoint_compile_basic_shaders TARGET_NAME OUTPUT_DIR)
    if(NOT TARGET bgfx::shaderc)
        message(STATUS "Waypoint shaders: bgfx::shaderc is unavailable; set WAYPOINT_BUILD_BGFX_TOOLS=ON to compile shaders.")
        return()
    endif()

    include("${bgfx_cmake_SOURCE_DIR}/cmake/bgfxToolUtils.cmake")
    set(BGFX_SHADER_INCLUDE_PATH "${bgfx_SOURCE_DIR}/src")

    set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shaders")
    set(VARYING_DEF "${SHADER_DIR}/varying.def.sc")

    bgfx_compile_shaders(
        TYPE VERTEX
        SHADERS "${SHADER_DIR}/vs_basic.sc"
        VARYING_DEF "${VARYING_DEF}"
        OUTPUT_DIR "${OUTPUT_DIR}"
        OUT_FILES_VAR VERTEX_OUTPUTS
        INCLUDE_DIRS "${BGFX_SHADER_INCLUDE_PATH}"
    )
    bgfx_compile_shaders(
        TYPE FRAGMENT
        SHADERS "${SHADER_DIR}/fs_basic.sc"
        VARYING_DEF "${VARYING_DEF}"
        OUTPUT_DIR "${OUTPUT_DIR}"
        OUT_FILES_VAR FRAGMENT_OUTPUTS
        INCLUDE_DIRS "${BGFX_SHADER_INCLUDE_PATH}"
    )

    add_custom_target(${TARGET_NAME} ALL DEPENDS ${VERTEX_OUTPUTS} ${FRAGMENT_OUTPUTS})
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "waypoint")
endfunction()
