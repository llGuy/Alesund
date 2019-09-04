#pragma once

#include "opengl_context.hpp"

struct game_memory_t
{
	vertex_buffer_t triangle_vertices;
};

void initialize_game();

// Render + update
void update_game();