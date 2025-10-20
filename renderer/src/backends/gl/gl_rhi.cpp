
#include "gl_rhi.hpp"

#include <iostream>
#include <ostream>
#include <glad/glad.h>

namespace oren {
    class GlRhi : public oren::IRhi {
    public:
        GlRhi() {

        }
        ~GlRhi() {

        }

        void init_context() override {
            if (!gladLoadGL())
                std::cerr << "Failed to load OpenGL context" << std::endl;

        }
        void begin_render(oren::Handle colorTarget, oren::Handle depthTarget) override {}

        void end_render() override {}

        void cmd_bind_buffer(oren::Handle h) override {}

        void cmd_bind_shader(oren::Handle h) override {}

        void cmd_bind_texture(oren::Handle h) override {}

        void cmd_unbind_buffer(oren::Handle h) override {}

        void cmd_unbind_shader(oren::Handle h) override {}

        void cmd_unbind_texture(oren::Handle h) override {}

        void cmd_destroy_buffer(Handle h) override {}
        void cmd_destroy_shader(Handle h) override {}
        void cmd_destroy_texture(Handle h) override {}

        void cmd_draw(oren::Handle h) override {}

        void cmd_draw_indexed(oren::Handle h) override {}

        oren::Handle cmd_create_buffer() override {
            return oren::Handle();
        }

        oren::Handle cmd_create_shader() override {
            return oren::Handle();
        }

        oren::Handle cmd_create_texture() override {
            return oren::Handle();
        }

        void cmd_clear() override {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void cmd_clear_color(const ColorDesc color) override {
            glClearColor(color.r, color.g, color.b, color.a);
        }
    };

    IRhi* make_gl_rhi() {
        auto rhi = new GlRhi();
        rhi->init_context();
        return rhi;
    }
}