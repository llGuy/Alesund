#include <stdint.h>
#include <cassert>
#include <windows.h>
#include <GL/gl.h>
#include <wglext.h>
#include <glext.h>
#include <cstring>
#include <stdint.h>

#include "opengl_context.hpp"

static HDC g_device_context;
static HGLRC g_opengl_render_context;

#define DECLARE_GL_PROC(pointer_name, function_name) pointer_name function_name
#define LOAD_GL_PROC(pointer_name, function_name) function_name = (pointer_name)wglGetProcAddress(#function_name);

DECLARE_GL_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
DECLARE_GL_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
DECLARE_GL_PROC(PFNGLBUFFERDATAPROC, glBufferData);
DECLARE_GL_PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
DECLARE_GL_PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
DECLARE_GL_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
DECLARE_GL_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
DECLARE_GL_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
DECLARE_GL_PROC(PFNGLCREATESHADERPROC, glCreateShader);
DECLARE_GL_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
DECLARE_GL_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
DECLARE_GL_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);
DECLARE_GL_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
DECLARE_GL_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
DECLARE_GL_PROC(PFNGLUNIFORM4FVPROC, glUniform4fv);
DECLARE_GL_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
DECLARE_GL_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
DECLARE_GL_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
DECLARE_GL_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
DECLARE_GL_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
DECLARE_GL_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
DECLARE_GL_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
DECLARE_GL_PROC(PFNGLDETACHSHADERPROC, glDetachShader);
DECLARE_GL_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData);

gpu_buffer_t create_gpu_buffer()
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	return (gpu_buffer_t)vbo;
}

void bind_gpu_buffer(GLenum target, gpu_buffer_t buffer)
{
	glBindBuffer(target, buffer);
}

// usage : GL_DRAW_STATIC, GL_DRAW_DYNAMIC
void update_gpu_buffer(GLenum target, void* data, int32_t data_size, GLenum usage)
{
	glBufferData(target, data_size, data, usage);
}

void update_gpu_buffer(GLenum target, int32_t offset, int32_t size, void* data)
{
	glBufferSubData(target, offset, size, data);
}

void reserve_buffer(GLenum target, int32_t size, GLenum usage, gpu_buffer_t buffer)
{
	bind_gpu_buffer(target, buffer);
	glBufferData(target, size, 0, usage);
}


vertex_layout_t create_vertex_layout()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	return (vertex_layout_t)vao;
}

void bind_vertex_layout(vertex_layout_t layout)
{
	glBindVertexArray(layout);
}

void enable_attribute_array(int32_t attribute_index)
{
	glEnableVertexAttribArray(attribute_index);
}

void define_attribute_layout(int32_t attribute_index, int32_t size, GLenum type, int32_t stride, int32_t starting_pointer)
{
	glVertexAttribPointer(attribute_index, size, type, GL_FALSE, stride, (void *)((int64_t)starting_pointer));
}


gpu_program_t create_gpu_program(shader_source_t* sources, int32_t source_count)
{
	typedef uint32_t gpu_shader_t;

	gpu_program_t program = glCreateProgram();

	const int32_t MAX_SHADERS = 5;
	gpu_shader_t shaders[MAX_SHADERS] = {};

	for (int32_t shader = 0; shader < source_count; ++shader)
	{
		shaders[shader] = glCreateShader(sources[shader].shader_type);
		int32_t source_length = (int32_t)strlen(sources[shader].source);

		glShaderSource(shaders[shader], 1, &sources[shader].source, &source_length);
		glCompileShader(shaders[shader]);

		GLint result = GL_FALSE;
		glGetShaderiv(shaders[shader], GL_COMPILE_STATUS, &result);

		if (!result)
		{
			// Get info
			OutputDebugString("Shader failed to compile");
			assert(0);
		}

		glAttachShader(program, shaders[shader]);
	}

	glLinkProgram(program);

	GLint result = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (!result)
	{
		// Get info
		OutputDebugString("Program failed to link");
		assert(0);
	}

	for (int32_t shader = 0; shader < source_count; ++shader)
	{
		glDetachShader(program, shaders[shader]);
	}

	return program;
}

void bind_gpu_program(gpu_program_t program)
{
	glUseProgram(program);
}

uint32_t get_uniform_location(const char *uniform_name, gpu_program_t gpu_program)
{
	return glGetUniformLocation(gpu_program, uniform_name);
}

void set_uniform(uint32_t uniform_location, void* data, uint32_t count, uniform_type_t type)
{
	switch (type)
	{

	case uniform_type_t::VECTOR4: {
		
		glUniform4fv(uniform_location, count, (GLfloat *)data);

	} break;
	case uniform_type_t::MATRIX4X4: {

		glUniformMatrix4fv(uniform_location, count, GL_FALSE, (GLfloat *)data);

	} break;

	}
}

void draw_arrays(GLenum primitive, int32_t starting_vertex, int32_t vertex_count)
{
	glDrawArrays(primitive, starting_vertex, vertex_count);
}

void draw_elements(GLenum primitive, index_layout_t index_layout)
{
	glDrawElements(primitive, index_layout.index_count, index_layout.index_type, (void *)((int64_t)index_layout.starting_byte));
}

void initialize_opengl_context(HWND window_handle)
{
	PIXELFORMATDESCRIPTOR pfd = {};

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	g_device_context = GetDC(window_handle);
	int32_t format_index = ChoosePixelFormat(g_device_context, &pfd);
	assert(format_index);
	assert(SetPixelFormat(g_device_context, format_index, &pfd));

	g_opengl_render_context = wglCreateContext(g_device_context);
	assert(g_opengl_render_context);

	assert(wglMakeCurrent(g_device_context, g_opengl_render_context));

	// Initialize all OpenGL extension functions
	LOAD_GL_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
	LOAD_GL_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
	LOAD_GL_PROC(PFNGLBUFFERDATAPROC, glBufferData);
	LOAD_GL_PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
	LOAD_GL_PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
	LOAD_GL_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
	LOAD_GL_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
	LOAD_GL_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
	LOAD_GL_PROC(PFNGLCREATESHADERPROC, glCreateShader);
	LOAD_GL_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
	LOAD_GL_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
	LOAD_GL_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
	LOAD_GL_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
	LOAD_GL_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
	LOAD_GL_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
	LOAD_GL_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
	LOAD_GL_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
	LOAD_GL_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
	LOAD_GL_PROC(PFNGLDETACHSHADERPROC, glDetachShader);
	LOAD_GL_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);
	LOAD_GL_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
	LOAD_GL_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
	LOAD_GL_PROC(PFNGLUNIFORM4FVPROC, glUniform4fv);
	LOAD_GL_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
}

void swap_buffers()
{
	SwapBuffers(g_device_context);
}
