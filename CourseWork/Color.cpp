#include "Color.h"
#include <math.h>


Color::Color(float a, float r, float b, float g) {
	this->a = a;
	this->r = r;
	this->g = g; 
	this->b = b;
}


Color::Color(Color *from) {
	this->from(from);
}


void Color::from(Color *color) {
	r = color->r;
	g = color->g;
	b = color->b;
	a = color->a;
}


void Color::overlap(Color *color) {
	r = r + (color->r - r) * color->a;
	g = g + (color->g - g) * color->a;
	b = b + (color->b - b) * color->a;
	a = fmaxf(color->a, a);
}