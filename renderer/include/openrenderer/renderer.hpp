//
// Created by nazarii on 10/19/25.
//

#ifndef OPENRENDER_RENDERER_HPP
#define OPENRENDER_RENDERER_HPP

#include "resources.hpp"


namespace oren {


    class Renderer {
    private:
        struct RendererImpl;
        RendererImpl* impl;
    public:
        Renderer(DeviceDesc device);
        ~Renderer();


        void beginFrame();
        void clearColor(ColorDesc color_desc);
        void endFrame();

    };


}

#endif //OPENRENDER_RENDERER_HPP