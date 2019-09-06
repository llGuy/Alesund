#pragma once

#include <windows.h>
#include <GL/gl.h>

typedef unsigned int gpu_buffer_t;
typedef unsigned int vertex_layout_t;
typedef unsigned int index_layout_t;
typedef unsigned int gpu_program_t;

gpu_buffer_t create_gpu_buffer();
void bind_gpu_buffer(GLenum target, gpu_buffer_t buffer);
void update_gpu_buffer(GLenum target, void *data, int data_size, GLenum usage);
void update_gpu_buffer(GLenum target, uint32_t offset, uint32_t size, void* data);
void reserve_buffer(GLsizeiptr size, GLenum usage);

vertex_layout_t create_vertex_layout();
void bind_vertex_layout(vertex_layout_t layout);
void enable_attribute_array(int attribute_index);
void define_attribute_layout(int attribute_index, int size /* NOT SIZE IN BYTES, SIZE IN TYPES */, GLenum type, int stride /* SIZE IN BYTES */, int starting_pointer);

struct shader_source_t
{
	// Fragment, Vertex, ...
	GLenum shader_type;
	const char *source;
};

gpu_program_t create_gpu_program(shader_source_t *sources, int source_count);
void bind_gpu_program(gpu_program_t program);

void draw_arrays(GLenum primitive, int starting_vertex, int vertex_count);

void initialize_opengl_context(HWND window_handle);
void swap_buffers();