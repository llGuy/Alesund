#pragma once

#include "opengl_context.hpp"

struct game_memory_t
{
	gpu_buffer_t triangle_vbo;
	vertex_layout_t triangle_layout;
	gpu_program_t triangle_program;
};

void initialize_game(game_memory_t *game);

// Render + update
void tick_game(game_memory_t *game);