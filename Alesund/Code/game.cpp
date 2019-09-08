#include "game.hpp"
#include "opengl_context.hpp"
#include "platform_core.hpp"

#include <stdint.h>
#include <windows.h>
#include <glext.h>
#include <wglext.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

void initialize_game(game_memory_t *memory)
{
	file_content_t vertex_shader_code = read_file("Code/Shaders/phong_lighting.vert");
	file_content_t fragment_shader_code = read_file("Code/Shaders/phong_lighting.frag");

	shader_source_t sources[2];
	sources[0].source = vertex_shader_code.bytes;
	sources[1].source = fragment_shader_code.bytes;
	sources[0].shader_type = GL_VERTEX_SHADER;
	sources[1].shader_type = GL_FRAGMENT_SHADER;

	memory->phong_shader = create_gpu_program(sources, 2);
	bind_gpu_program(memory->phong_shader);

	memory->projection_matrix_uloc = get_uniform_location("projection_matrix", memory->phong_shader);
	memory->view_matrix_uloc = get_uniform_location("view_matrix", memory->phong_shader);
	memory->model_matrix_uloc = get_uniform_location("model_matrix", memory->phong_shader);
	memory->color_uloc = get_uniform_location("color", memory->phong_shader);

	memory->projection_matrix = glm::perspective(glm::radians(60.0f), (float)memory->viewport_x / (float)memory->viewport_y, 0.1f, 10000.0f);
	memory->view_matrix = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(5.0f, 5.0f, 5.0f) + glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	memory->model_matrix = glm::translate(glm::vec3(0.0f));
	memory->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	set_uniform(memory->projection_matrix_uloc, &memory->projection_matrix[0][0], 1, uniform_type_t::MATRIX4X4);
	set_uniform(memory->view_matrix_uloc, &memory->view_matrix[0][0], 1, uniform_type_t::MATRIX4X4);
	set_uniform(memory->model_matrix_uloc, &memory->model_matrix[0][0], 1, uniform_type_t::MATRIX4X4);
	set_uniform(memory->color_uloc, &memory->color[0], 1, uniform_type_t::VECTOR4);
	
	// cube vertices
	memory->vertex_buffer = create_gpu_buffer();
	bind_gpu_buffer(GL_ARRAY_BUFFER, memory->vertex_buffer);

	float radius = 1.0f;

	glm::vec3 vertices[]
	{
		{-radius, -radius, radius },
		{radius, -radius, radius },
		{radius, radius, radius	},
		{-radius, radius, radius },

		{-radius, -radius, -radius },
		{radius, -radius, -radius },
		{radius, radius, -radius },
		{-radius, radius, -radius }
	};

	update_gpu_buffer(GL_ARRAY_BUFFER, vertices, sizeof(vertices), GL_STATIC_DRAW);

	memory->vertex_layout = create_vertex_layout();
	bind_vertex_layout(memory->vertex_layout);
	enable_attribute_array(0);
	define_attribute_layout(0, 3, GL_FLOAT, 3 * sizeof(float), 0);

	// Cube indices
	memory->index_buffer = create_gpu_buffer();
	bind_gpu_buffer(GL_ELEMENT_ARRAY_BUFFER, memory->index_buffer);

	uint32_t mesh_indices[] =
	{
		0, 1, 2,
		2, 3, 0,

		1, 5, 6,
		6, 2, 1,

		7, 6, 5,
		5, 4, 7,

		3, 7, 4,
		4, 0, 3,

		4, 5, 1,
		1, 0, 4,

		3, 2, 6,
		6, 7, 3,
	};

	update_gpu_buffer(GL_ELEMENT_ARRAY_BUFFER, mesh_indices, sizeof(mesh_indices), GL_STATIC_DRAW);
	memory->index_layout.index_count = sizeof(mesh_indices) / sizeof(uint32_t);
	memory->index_layout.index_type = GL_UNSIGNED_INT;
	memory->index_layout.starting_byte = 0;
}

// To render triangle
// Create a vertex buffer with vertices, colors
// Create a vertex array object -> layout of our vertex buffer
// Create a shader to shade our triangle
// RENDER!

void tick_game(game_memory_t *memory)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	draw_elements(GL_TRIANGLES, memory->index_layout);

	swap_buffers();
}