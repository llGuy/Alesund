#include "game.hpp"
#include "opengl_context.hpp"

#include <windows.h>
#include <glext.h>
#include <wglext.h>
#include <GL/gl.h>

void initialize_game(game_memory_t *memory)
{
	// Create vertex buffer
	memory->triangle_vbo = create_gpu_buffer();
	bind_gpu_buffer(GL_ARRAY_BUFFER, memory->triangle_vbo);
	float vertices[] = { -0.5f, -0.5f, 0.0f, +0.5f, +0.5f, -0.5f };
	update_gpu_buffer(GL_ARRAY_BUFFER, vertices, sizeof(vertices), GL_STATIC_DRAW);

	// Create square's vertex buffer
	memory->square_vbo = create_gpu_buffer();

	// Create vertex layout
	memory->triangle_layout = create_vertex_layout();
	bind_vertex_layout(memory->triangle_layout);
	enable_attribute_array(0);
	define_attribute_layout(0, 2, GL_FLOAT, sizeof(float) * 2, 0);

	shader_source_t sources[2];
	sources[0].shader_type = GL_VERTEX_SHADER;
	sources[0].source = R"sh(#version 400
	   layout(location = 0) in vec2 vertex_position;
	void main() {
		gl_Position = vec4(vertex_position, 0.0, 1.0);
	})sh";
	sources[1].shader_type = GL_FRAGMENT_SHADER;
	sources[1].source = R"sh(#version 400
		out vec4 final_color;
		void main() {
			final_color = vec4(0.0);
	})sh";

	memory->triangle_program = create_gpu_program(sources, 2);
}

// To render triangle
// Create a vertex buffer with vertices, colors
// Create a vertex array object -> layout of our vertex buffer
// Create a shader to shade our triangle
// RENDER!

void tick_game(game_memory_t *memory)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Bind shader
	bind_gpu_program(memory->triangle_program);

	// Bind vertex layout
	bind_vertex_layout(memory->triangle_layout);

	// Draw
	draw_arrays(GL_TRIANGLES, 0, 3);

	swap_buffers();
}