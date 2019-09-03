/* The beginning of a journey */

#include <cassert>
#include <stdint.h>
#include <windows.h>

LRESULT CALLBACK window_callback(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	return 0;
}

int WinMain(HINSTANCE hinstance, HINSTANCE previous_instance, LPSTR cmdline, int showcmd)
{
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

	ShowWindow(window, showcmd);

	while(true)
	{
		
	}

	return 0;
}