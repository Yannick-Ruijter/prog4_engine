#pragma once

#include <iostream>

consteval unsigned int operator ""_h(const char* str, size_t) {
	unsigned int hash = 0;
	int c;
	while ((c = *str++))
	{
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}