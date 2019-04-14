#include "Canvas.h"
#include <stdlib.h>



Canvas::Canvas()
{
}


Canvas::~Canvas()
{
	delete this->canvas;
}


Canvas::Canvas(int width, int height) {
	resize(width, height);
}


void Canvas::resize(int width, int height) {
	this->width = width;
	this->height = height;
	this->canvas = (Color*) realloc(this->canvas, sizeof(Color) * width * height);
}


int Canvas::getWidth() {
	return width;
}


int Canvas::getHeight() {
	return height;
}


Color* Canvas::get(int x, int y) {
	return &canvas[y * width + x];
}


void Canvas::fill(Color *color) {
	int total = width * height;
	for (int i = 0; i < total; i++) {
		canvas[i].from(color);
	}
}
