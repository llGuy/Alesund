#pragma once

#include "opengl_context.hpp"

struct game_memory_t
{
	gpu_buffer_t triangle_vbo;
	vertex_layout_t triangle_layout;
	gpu_program_t triangle_program;

	gpu_buffer_t square_vbo;
	vertex_layout_t square_vertex_layout;
	index_layout_t square_index_layout;
};

void initialize_game(game_memory_t *game);

// Render + update
void tick_game(game_memory_t *game);