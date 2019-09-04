/* The beginning of a journey */

#include <cassert>
#include <stdint.h>
#include <windows.h>

#include <GL/gl.h>
#include <wglext.h>

bool g_running;
HDC g_device_context;
HGLRC g_opengl_render_context;

LRESULT CALLBACK window_callback(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{

	case WM_KEYDOWN: {
	} break;

	}

	return DefWindowProc(window_handle, message, wparam, lparam);
}

void initialize_graphics_context(HWND window_handle, HINSTANCE instance)
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
}

void render_game()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	SwapBuffers(g_device_context);
}

int CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE previous_instance, LPSTR cmdline, int showcmd)
{
	g_running = 0;

	const char *window_class_name = "alesund_window_class";
	const char *window_name = "Alesund";

	WNDCLASS window_class = {};
	window_class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	window_class.hInstance = hinstance;
	window_class.lpszClassName = window_class_name;
	window_class.lpfnWndProc = window_callback;

	assert(RegisterClass(&window_class));

	HWND window = CreateWindowEx(0,
		window_class_name,
		window_name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hinstance,
		NULL);

	assert(window);

	initialize_graphics_context(window, hinstance);

	ShowWindow(window, showcmd);

	g_running = 1;

	while (g_running)
	{
		// Frame
		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		render_game();
	}

	return 0;
}