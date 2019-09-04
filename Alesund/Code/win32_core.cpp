/* The beginning of a journey */

#include <cassert>
#include <stdint.h>
#include <windows.h>

#include <GL/gl.h>

#include "opengl_context.hpp"

// GAME CODE HERE
#include "game.hpp"

bool g_running;

LRESULT CALLBACK window_callback(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{

	case WM_CLOSE: {
		g_running = 0;
		PostQuitMessage(0);
	} break;

	}

	return DefWindowProc(window_handle, message, wparam, lparam);
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

	initialize_opengl_context(window);

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

		update_game();
	}

	return 0;
}