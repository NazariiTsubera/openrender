#include "gl_rhi.hpp"

#include <glad/glad.h>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

namespace oren {

  static GLenum to_gl_target(BufferType t) {
    switch (t) {
      case BufferType::INDEX:   return GL_ELEMENT_ARRAY_BUFFER;
      case BufferType::VERTEX:  return GL_ARRAY_BUFFER;
      case BufferType::UNIFORM: return GL_UNIFORM_BUFFER;
    }
    return GL_ARRAY_BUFFER;
  }

  struct fmt_info { GLint comps; GLenum type; GLboolean norm; bool is_int; };
  static fmt_info to_gl_format(AttribFormat f) {
    switch (f) {
      case AttribFormat::r32_float:          return {1, GL_FLOAT,         GL_FALSE, false};
      case AttribFormat::r32g32_float:       return {2, GL_FLOAT,         GL_FALSE, false};
      case AttribFormat::r32g32b32_float:    return {3, GL_FLOAT,         GL_FALSE, false};
      case AttribFormat::r32g32b32a32_float: return {4, GL_FLOAT,         GL_FALSE, false};
      case AttribFormat::r8g8b8a8_unorm:     return {4, GL_UNSIGNED_BYTE, GL_TRUE,  false};
      case AttribFormat::r32_uint:           return {1, GL_UNSIGNED_INT,  GL_FALSE, true };
      case AttribFormat::r32_sint:           return {1, GL_INT,           GL_FALSE, true };
    }
    return {3, GL_FLOAT, GL_FALSE, false};
  }

  static GLuint compile_shader(GLenum stage, const std::string& src) {
    GLuint s = glCreateShader(stage);
    const char* p = src.c_str();
    glShaderSource(s, 1, &p, nullptr);
    glCompileShader(s);
    GLint ok = GL_FALSE;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
      GLint len = 0; glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
      std::vector<char> log(len ? len : 1);
      glGetShaderInfoLog(s, len, nullptr, log.data());
      std::cerr << "[GL] Shader compile failed:\n" << log.data() << std::endl;
    }
    return s;
  }

  static GLuint link_program(GLuint vs, GLuint fs) {
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    GLint ok = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
      GLint len = 0; glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
      std::vector<char> log(len ? len : 1);
      glGetProgramInfoLog(prog, len, nullptr, log.data());
      std::cerr << "[GL] Program link failed:\n" << log.data() << std::endl;
    }
    glDetachShader(prog, vs);
    glDetachShader(prog, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
  }

  static GLint compute_unpack_alignment(GLsizei width, GLint bytes_per_pixel) {
    GLint row = width * bytes_per_pixel;
    if (row % 8 == 0) return 8;
    if (row % 4 == 0) return 4;
    if (row % 2 == 0) return 2;
    return 1;
  }


  class GlRhi : public oren::IRhi {
  public:
    GlRhi() = default;
    ~GlRhi() override = default;

    void init_context() override {
      if (!gladLoadGL()) {
        std::cerr << "Failed to load OpenGL context" << std::endl;
      }
      glEnable(GL_DEPTH_TEST);
    }

    void begin_render(Handle, Handle) override {}
    void end_render() override {}

    // binds (explicit, but not cached)
    void cmd_bind_shader(ShaderHandle h) override { glUseProgram((GLuint)h); }
    void cmd_unbind_shader(ShaderHandle) override { glUseProgram(0); }

    void cmd_bind_texture(TextureHandle h) override {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, (GLuint)h);
    }
    void cmd_unbind_texture(TextureHandle) override { glBindTexture(GL_TEXTURE_2D, 0); }

    BufferHandle cmd_create_buffer(BufferDesc desc) override {
      const GLenum target = to_gl_target(desc.type);
      GLuint buf = 0;
      glGenBuffers(1, &buf);
      glBindBuffer(target, buf);
      glBufferData(target, (GLsizeiptr)desc.size, desc.data, GL_STATIC_DRAW);
      if (target != GL_ELEMENT_ARRAY_BUFFER) glBindBuffer(target, 0);
      return (BufferHandle)buf;
    }

    ShaderHandle cmd_create_shader(ShaderDesc desc) override {
      GLuint vs = compile_shader(GL_VERTEX_SHADER,   desc.vertex);
      GLuint fs = compile_shader(GL_FRAGMENT_SHADER, desc.fragment);
      return (ShaderHandle)link_program(vs, fs);
    }

    TextureHandle cmd_create_texture(TextureDesc desc) override {
      GLuint tex = 0;
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);

      const bool has = desc.data && desc.width && desc.height;
      glPixelStorei(GL_UNPACK_ALIGNMENT,
        has ? compute_unpack_alignment((GLsizei)desc.width, 4) : 1);

      if (has) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     (GLsizei)desc.width, (GLsizei)desc.height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, desc.data);
      } else {
        unsigned char white[4] = {255,255,255,255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
      }

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glGenerateMipmap(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);
      return (TextureHandle)tex;
    }

    VertexInputHandle cmd_create_vertex_input(std::span<VertexAttribDesc> attrs, IndexDesc idx) override {
      GLuint vao = 0;
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      if (idx.buffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)idx.buffer);
      }

      for (const auto& a : attrs) {
        const auto fi = to_gl_format(a.format);
        glBindBuffer(GL_ARRAY_BUFFER, (GLuint)a.handle);
        glEnableVertexAttribArray(a.location);
        const void* ptr = reinterpret_cast<const void*>((uintptr_t)a.offset);
        if (fi.is_int) {
          glVertexAttribIPointer(a.location, fi.comps, fi.type, (GLsizei)a.stride, ptr);
        } else {
          glVertexAttribPointer(a.location, fi.comps, fi.type, fi.norm, (GLsizei)a.stride, ptr);
        }
        glVertexAttribDivisor(a.location, a.divisor);
      }

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
      return (VertexInputHandle)vao;
    }

    void cmd_destroy_buffer(BufferHandle h) override {
      if (!h) return;
      GLuint id = (GLuint)h;
      glDeleteBuffers(1, &id);
    }
    void cmd_destroy_texture(TextureHandle h) override {
      if (!h) return;
      GLuint id = (GLuint)h;
      glDeleteTextures(1, &id);
    }
    void cmd_destroy_shader(ShaderHandle h) override {
      if (!h) return;
      glDeleteProgram((GLuint)h);
    }
    void cmd_destroy_vertex_input(VertexInputHandle h) override {
      if (!h) return;
      GLuint vao = (GLuint)h;
      glDeleteVertexArrays(1, &vao);
    }

    void cmd_draw(VertexInputHandle vi,
                  uint32_t vertex_count,
                  uint32_t first_vertex) override {
      glBindVertexArray((GLuint)vi);
      glDrawArrays(GL_TRIANGLES, (GLint)first_vertex, (GLsizei)vertex_count);
      glBindVertexArray(0);
    }

    void cmd_draw_indexed(VertexInputHandle vi,
                          IndexType        index_type,
                          uint32_t         index_count,
                          uint32_t         first_index,
                          int32_t          vertex_offset,
                          uint32_t         index_byte_offset) override {
      glBindVertexArray((GLuint)vi);

      const GLsizei elem_size = (index_type == IndexType::u16) ? 2 : 4;
      const GLenum  gl_type   = (index_type == IndexType::u16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
      const GLvoid* start     = reinterpret_cast<const void*>(
                                  (size_t)index_byte_offset + (size_t)first_index * elem_size);


      glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)index_count, gl_type, start, vertex_offset);
      glBindVertexArray(0);
    }

    void cmd_clear() override { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    void cmd_clear_color(const ColorDesc c) override { glClearColor(c.r, c.g, c.b, c.a); }
  };

  // factory
  IRhi* make_gl_rhi() {
    auto rhi = new GlRhi();
    rhi->init_context();
    return rhi;
}

} // namespace oren
