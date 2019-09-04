#include "game.hpp"
#include "opengl_context.hpp"

#include <GL/gl.h>

void initialize_game()
{
	// Create vertex buffer
	
}

// To render triangle
// Create a vertex buffer with vertices, colors
// Create a vertex array object -> layout of our vertex buffer
// Create a shader to shade our triangle
// RENDER!

void update_game()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	swap_buffers();
}