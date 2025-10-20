// renderer.cpp
#include "openrenderer/renderer.hpp"
#include "rhi/rhi.hpp"        // IRhi, descriptors, handles
#include "backends/gl/gl_rhi.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace oren {

  static std::unique_ptr<IRhi> make_rhi_from_device(const DeviceDesc& device) {
    switch (device.backend) {
      case Backend::OpenGL:
      default:
        return std::unique_ptr<IRhi>(make_gl_rhi());
    }
  }

  struct Renderer::RendererImpl {
    std::unique_ptr<IRhi> rhi;

    ShaderHandle        current_shader{0};
    TextureHandle       current_texture{0};

    explicit RendererImpl(const DeviceDesc& device)
    : rhi(make_rhi_from_device(device)) {
    }
  };

  Renderer::Renderer(DeviceDesc device)
  : impl(new RendererImpl(device)) {}

  Renderer::~Renderer() {
    delete impl;
    impl = nullptr;
  }

  void Renderer::begin_frame() {
    impl->rhi->begin_render(0, 0);
  }

  void Renderer::clear_color(ColorDesc c) {
    impl->rhi->cmd_clear_color(c);
  }

  void Renderer::clear() {
    impl->rhi->cmd_clear();
  }

  void Renderer::end_frame() {
    impl->rhi->end_render();
  }


  BufferHandle Renderer::create_buffer(const BufferDesc& d) {
    return impl->rhi->cmd_create_buffer(d);
  }

  TextureHandle Renderer::create_texture(const TextureDesc& d) {
    return impl->rhi->cmd_create_texture(d);
  }

  ShaderHandle Renderer::create_shader(const ShaderDesc& d) {
    return impl->rhi->cmd_create_shader(d);
  }

  VertexInputHandle Renderer::create_vertex_input(std::span<const VertexAttribDesc> attrs, IndexDesc idx) {
    std::vector<VertexAttribDesc> tmp(attrs.begin(), attrs.end());
    std::span<VertexAttribDesc>   span_nc(tmp.data(), tmp.size());
    return impl->rhi->cmd_create_vertex_input(span_nc, idx);
  }

  void Renderer::destroy_buffer(BufferHandle h) {
    impl->rhi->cmd_destroy_buffer(h);
  }

  void Renderer::destroy_texture(TextureHandle h) {
    impl->rhi->cmd_destroy_texture(h);
  }

  void Renderer::destroy_shader(ShaderHandle h) {
    impl->rhi->cmd_destroy_shader(h);
  }

  void Renderer::destroy_vertex_input(VertexInputHandle h) {
    impl->rhi->cmd_destroy_vertex_input(h);
  }


  void Renderer::bind_shader(ShaderHandle h) {
    impl->rhi->cmd_bind_shader(h);
    impl->RendererImpl::current_shader = h;
  }

  void Renderer::bind_texture(TextureHandle h) {
    impl->rhi->cmd_bind_texture(h);
    impl->RendererImpl::current_texture = h;
  }

  void Renderer::draw(VertexInputHandle h, uint32_t vertex_count, uint32_t first_vertex) {
    impl->rhi->cmd_draw(h,vertex_count, first_vertex);
  }

  void Renderer::draw_indexed(VertexInputHandle vi, IndexType index_type, uint32_t index_count, uint32_t first_index, int32_t vertex_offset, uint32_t index_byte_offset){
    impl->rhi->cmd_draw_indexed(vi, index_type, index_count, first_index, vertex_offset, index_byte_offset);
  }

}
