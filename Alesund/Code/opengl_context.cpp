#include <cassert>
#include <windows.h>
#include <GL/gl.h>
#include <wglext.h>
#include <glext.h>

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
DECLARE_GL_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
DECLARE_GL_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
DECLARE_GL_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
DECLARE_GL_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
DECLARE_GL_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
DECLARE_GL_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
DECLARE_GL_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
DECLARE_GL_PROC(PFNGLDETACHSHADERPROC, glDetachShader);

vertex_buffer_t create_vertex_buffer()
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	return (vertex_buffer_t)vbo;
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
	int format_index = ChoosePixelFormat(g_device_context, &pfd);
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
}

void swap_buffers()
{
	SwapBuffers(g_device_context);
}