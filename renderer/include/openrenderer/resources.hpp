//
// Created by nazarii on 10/19/25.
//

#ifndef OPENRENDER_RESOURCES_HPP
#define OPENRENDER_RESOURCES_HPP
#include <cstdint>

namespace oren {

    struct ColorDesc { float r, g, b, a; };

    struct DeviceDesc {
        enum Backend {
            OpenGL
        };

        Backend backend;
        void* context;
    };

    struct PipelineDesc {

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