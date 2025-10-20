//
// Created by nazarii on 10/19/25.
//

#ifndef OPENRENDER_RHI_HPP

#define OPENRENDER_RHI_HPP
#include <span>

#include "openrenderer/resources.hpp"

namespace oren {

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

        virtual void cmd_bind_shader(ShaderHandle) = 0;
        virtual void cmd_bind_texture(TextureHandle) = 0;

        virtual void cmd_unbind_shader(ShaderHandle) = 0;
        virtual void cmd_unbind_texture(TextureHandle) = 0;


        virtual void cmd_draw(VertexInputHandle h, uint32_t vertex_count, uint32_t first_vertex) = 0;
        virtual void cmd_draw_indexed(VertexInputHandle vi, IndexType index_type, uint32_t index_count, uint32_t first_index, int32_t vertex_offset, uint32_t index_byte_offset) = 0;


        virtual BufferHandle cmd_create_buffer(BufferDesc) = 0;
        virtual ShaderHandle cmd_create_shader(ShaderDesc) = 0;
        virtual TextureHandle cmd_create_texture(TextureDesc) = 0;
        virtual VertexInputHandle cmd_create_vertex_input(std::span<VertexAttribDesc> attrs, IndexDesc idx) = 0;

        virtual void cmd_clear() = 0;
        virtual void cmd_clear_color(ColorDesc) = 0;

        virtual void cmd_destroy_buffer(BufferHandle) = 0;
        virtual void cmd_destroy_texture(TextureHandle) = 0;
        virtual void cmd_destroy_shader(ShaderHandle) = 0;
        virtual void cmd_destroy_vertex_input(VertexInputHandle) = 0;
    };

}
#endif //OPENRENDER_RHI_HPP