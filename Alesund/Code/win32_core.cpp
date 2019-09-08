/* The beginning of a journey */

#include <stdint.h>
#include <cassert>
#include <stdint.h>
#include <windows.h>
#include <memory.h>

#include <GL/gl.h>

#include "opengl_context.hpp"

// GAME CODE HERE
#include "game.hpp"
#include "platform_core.hpp"

bool g_running;

file_content_t read_file(const char* file_name)
{
	HANDLE file_handle = CreateFileA(
		file_name,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	assert(file_handle);

	uint32_t file_size = GetFileSize(file_handle, NULL);
	char *data = (char *)malloc(sizeof(char) * (file_size + 1));

	uint32_t bytes_read = 0;
	BOOL result = ReadFile(
		file_handle,
		(void *)data,
		file_size,
		(LPDWORD)&bytes_read,
		NULL
	);

	assert(result);

	data[file_size] = 0;

	file_content_t ret = {};
	ret.bytes = data;
	ret.file_name = file_name;
	ret.file_size = file_size;

	return ret;
}

void free_from_heap(void *data)
{
	free(data);
}

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

int32_t CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE previous_instance, LPSTR cmdline, int32_t showcmd)
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

	RECT rect = {};
	GetClientRect(window, &rect);

	assert(window);

	game_memory_t game = {};
	game.viewport_x = rect.right;
	game.viewport_y = rect.bottom;

	initialize_opengl_context(window);
	initialize_game(&game);

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

		tick_game(&game);
	}

	return 0;
}
