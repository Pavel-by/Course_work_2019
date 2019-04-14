#include "Point.h"
#include <math.h>


Point::Point(float x, float y) {
	set(x, y);
}


void Point::set(float x, float y) {
	this->x = x;
	this->y = y;
}


float Point::distance(Point *point) {
	float distance = sqrt((x - point->x) * (x - point->x) + (y - point->y) * (y - point->y));
	if (distance < 0)
	{
		distance = -distance;
	}
	return distance;
}