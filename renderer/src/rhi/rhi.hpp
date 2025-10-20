//
// Created by nazarii on 10/19/25.
//

#ifndef OPENRENDER_RHI_HPP
#define OPENRENDER_RHI_HPP
#include "openrenderer/renderer.hpp"

namespace oren {

    typedef uint32_t Handle;
    /**
     * RHI (Rendering Hardware Interface)
     * Internal low-level rendering API wrapper
     */
    class IRhi {
    public:
        virtual ~IRhi() = default;

        virtual void init_context() = 0;
        virtual void begin_render(Handle colorTarget, Handle depthTarget) = 0;
        virtual void end_render() = 0;

        virtual void cmd_bind_buffer(Handle) = 0;
        virtual void cmd_bind_shader(Handle) = 0;
        virtual void cmd_bind_texture(Handle) = 0;

        virtual void cmd_unbind_buffer(Handle) = 0;
        virtual void cmd_unbind_shader(Handle) = 0;
        virtual void cmd_unbind_texture(Handle) = 0;

        virtual void cmd_draw(Handle) = 0;
        virtual void cmd_draw_indexed(Handle) = 0;

        virtual Handle cmd_create_buffer() = 0;
        virtual Handle cmd_create_shader() = 0;
        virtual Handle cmd_create_texture() = 0;

        virtual void cmd_clear() = 0;
        virtual void cmd_clear_color(ColorDesc) = 0;

        virtual void cmd_destroy_buffer(Handle) = 0;
        virtual void cmd_destroy_texture(Handle) = 0;
        virtual void cmd_destroy_shader(Handle) = 0;
    };

}
#endif //OPENRENDER_RHI_HPP