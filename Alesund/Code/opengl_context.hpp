#pragma once

#include <windows.h>

typedef unsigned int vertex_buffer_t;
typedef unsigned int vertex_array_t;

vertex_buffer_t create_vertex_buffer();

void initialize_opengl_context(HWND window_handle);
void swap_buffers();