//
// Created by nazarii on 10/19/25.
//

#ifndef OPENRENDER_RENDERER_HPP
#define OPENRENDER_RENDERER_HPP

#include <span>

#include "resources.hpp"


namespace oren {




    class Renderer {
    private:
        struct RendererImpl;
        RendererImpl* impl;
    public:
        Renderer(DeviceDesc device);
        ~Renderer();


        void begin_frame();
        void clear_color(ColorDesc);
        void clear();
        void end_frame();

        BufferHandle      create_buffer(const BufferDesc&);
        TextureHandle     create_texture(const TextureDesc&);
        ShaderHandle      create_shader(const ShaderDesc&);
        VertexInputHandle create_vertex_input(std::span<const VertexAttribDesc>, IndexDesc);

        void              destroy_buffer(BufferHandle);
        void              destroy_texture(TextureHandle);
        void              destroy_shader(ShaderHandle);
        void              destroy_vertex_input(VertexInputHandle);


        void bind_shader(ShaderHandle);
        void bind_texture(TextureHandle);
        void draw(VertexInputHandle h, uint32_t vertex_count, uint32_t first_vertex);
        void draw_indexed(VertexInputHandle vi, IndexType index_type, uint32_t index_count, uint32_t first_index, int32_t vertex_offset, uint32_t index_byte_offset);

    };



}

#endif //OPENRENDER_RENDERER_HPP