#pragma once

#include <stdint.h>
#include "opengl_context.hpp"

#include <glm/glm.hpp>

struct game_memory_t
{
	uint32_t viewport_x, viewport_y;

	gpu_program_t phong_shader;
	uint32_t projection_matrix_uloc;
	uint32_t view_matrix_uloc;
	uint32_t model_matrix_uloc;
	uint32_t color_uloc;

	gpu_buffer_t vertex_buffer;
	gpu_buffer_t index_buffer;

	vertex_layout_t vertex_layout;
	index_layout_t index_layout;

	// Uniforms
	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 model_matrix;
	glm::vec4 color;
};

void initialize_game(game_memory_t *game);

// Render + update
void tick_game(game_memory_t *game);