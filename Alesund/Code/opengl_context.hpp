#pragma once

#include <stdint.h>
#include <windows.h>
#include <GL/gl.h>

typedef uint32_t gpu_buffer_t;
typedef uint32_t vertex_layout_t;

struct index_layout_t
{
	int32_t index_count;
	GLenum index_type;
	int32_t starting_byte;
};

typedef uint32_t gpu_program_t;

gpu_buffer_t create_gpu_buffer();
void bind_gpu_buffer(GLenum target, gpu_buffer_t buffer);
void update_gpu_buffer(GLenum target, void *data, int32_t data_size, GLenum usage);
//void update_gpu_buffer(GLenum target, int32_t offset, int32_t size, void* data);
void reserve_buffer(int32_t size, GLenum usage);

vertex_layout_t create_vertex_layout();
void bind_vertex_layout(vertex_layout_t layout);
void enable_attribute_array(int32_t attribute_index);
void define_attribute_layout(int32_t attribute_index, int32_t size /* NOT SIZE IN BYTES, SIZE IN TYPES */, GLenum type, int32_t stride /* SIZE IN BYTES */, int32_t starting_pointer);

struct shader_source_t
{
	// Fragment, Vertex, ...
	GLenum shader_type;
	const char *source;
};

gpu_program_t create_gpu_program(shader_source_t *sources, int32_t source_count);
void bind_gpu_program(gpu_program_t program);
uint32_t get_uniform_location(const char *uniform_name, gpu_program_t gpu_program);

enum uniform_type_t { VECTOR4, MATRIX4X4 };

void set_uniform(uint32_t uniform_location, void *data, uint32_t count, uniform_type_t type);

// Rarely will we use this, except for UI
void draw_arrays(GLenum primitive, int32_t starting_vertex, int32_t vertex_count);
void draw_elements(GLenum primitive, index_layout_t index_layout);

void initialize_opengl_context(HWND window_handle);
void swap_buffers();
