#include "Vector.h"
#include "Constants.h"



Vector::Vector() {}


Vector::Vector(float x, float y) {
	set(x, y);
}


void Vector::set(float x, float y) {
	this->x = x;
	this->y = y;
}


Vector* Vector::calculateNormal() {
	if (y < NUMBER_ACCURACY) return new Vector(0, 10);
	return new Vector(10, 10 * x / y);
}