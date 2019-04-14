#include "common.h"



int pullIntValue(const char *from, const int len) {
	int result = 0;
	strcpy_force((char*)&result, from, len > 4 ? 4 : len);
	return result;
}



void strcpy_force(char *dest, const char *source, int count) {
	for (int i = 0; i < count; i++) dest[i] = source[i];
}
