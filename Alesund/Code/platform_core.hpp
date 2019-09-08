#pragma once

#include <stdint.h>

struct file_content_t
{
	const char *file_name;
	const char *bytes;
	uint32_t file_size;
};

file_content_t read_file(const char *file_name);

void free_from_heap(void *data);