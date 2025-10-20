//
// Created by nazarii on 10/19/25.
//

#include "openrenderer/renderer.hpp"

#include "backends/gl/gl_rhi.hpp"
#include "rhi/rhi.hpp"

namespace oren {
    struct Renderer::RendererImpl {
        IRhi* rhi;
    };

    Renderer::Renderer(DeviceDesc device) {
        impl->rhi = make_gl_rhi();
    }


    Renderer::~Renderer() {

    }

    void Renderer::beginFrame() {
        impl->rhi->cmd_clear();
    }

    void Renderer::clearColor(ColorDesc color_desc) {
        this->impl->rhi->cmd_clear_color(color_desc);
    }

    void Renderer::endFrame() {
    }

}
