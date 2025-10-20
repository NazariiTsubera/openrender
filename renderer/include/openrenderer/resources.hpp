//
// Created by nazarii on 10/19/25.
//

#ifndef OPENRENDER_RESOURCES_HPP
#define OPENRENDER_RESOURCES_HPP
#include <cstdint>
#include <string>

namespace oren {

    typedef uint32_t Handle;
    typedef uint32_t VertexInputHandle;
    typedef uint32_t TextureHandle;
    typedef uint32_t BufferHandle;
    typedef uint32_t ShaderHandle;



    struct ShaderDesc {
        std::string vertex;
        std::string fragment;
    };

    struct TextureDesc {
        const void* data;
        uint32_t width;
        uint32_t height;
    };

    enum class BufferType : uint32_t
    {
        INDEX = 1u << 0,
        VERTEX = 1u << 1,
        UNIFORM = 1u << 2,
    };


    enum class IndexType : uint8_t { u16, u32 };

    enum class AttribFormat : uint32_t {
        r32_float, r32g32_float,
        r32g32b32_float, r32g32b32a32_float,
        r8g8b8a8_unorm, r32_uint, r32_sint
    };

    struct VertexAttribDesc {
        BufferHandle handle;
        uint8_t      location;
        AttribFormat format;
        uint32_t     stride;
        uint32_t     offset;
        uint32_t     divisor;
    };

    struct IndexDesc {
        BufferHandle  buffer{0};
        IndexType     type{IndexType::u32};
        uint32_t      offset{0};
    };


    struct BufferDesc {
        void* data;
        std::size_t size;
        BufferType type;
    };


    struct ColorDesc { float r, g, b, a; };

    struct MeshDesc {

    };

    struct MaterialDesc {

    };

    enum Backend {
        OpenGL
    };


    struct DeviceDesc {

        Backend backend;
        void* native;
    };



    struct Resource {

        enum Type {
            Buffer,
            Texture,
            Pipeline,
            Shader
        };

        uint32_t handle;
        Type type;

    };
}

#endif //OPENRENDER_RESOURCES_HPP