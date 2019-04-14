#include "Circle.h"
#include "Constants.h"
#include <math.h>



Circle::Circle(Point *center)
{
	this->center = center;
}


Circle::Circle(Point *center, int radius) {
	this->center = center;
	this->radius = radius;
}


Circle::Circle(Point *center, int radius, Color *color) {
	this->center = center;
	this->radius = radius;
	this->color = color;
}


int Circle::drawHorizontalLine(Canvas *canvas, int line) {
	int changedCount = 0, 
		width = canvas->getWidth(),
		height = canvas->getHeight();
	Point point = *center;
	Color tmpColor;
	tmpColor.from(color);
	float distance;
	point.y = line;
	point.x = fmaxf(0, center->x);
	distance = point.distance(center);
	while (distance < radius + ANTIALIAS && width > point.x) {
		tmpColor.a = distance <= radius ? color->a : color->a * (ANTIALIAS - (distance - radius));
		canvas->get(point.x, point.y)->overlap(&tmpColor);
		point.x++; 
		distance = point.distance(center);
		changedCount++;
	}
	point.x = fminf(width - 1, center->x - 1);
	distance = point.distance(center);
	while (distance < radius + ANTIALIAS && 0 <= point.x) {
		tmpColor.a = distance <= radius ? color->a : color->a * (ANTIALIAS - (distance - radius));
		canvas->get(point.x, point.y)->overlap(&tmpColor);
		point.x--;
		distance = point.distance(center);
		changedCount++;
	}
	return changedCount;
}


void Circle::draw(Canvas *canvas) {
	int height = canvas->getHeight(),
		curLine = center->y;
	while (curLine >= 0 && drawHorizontalLine(canvas, curLine)) curLine--;
	curLine = center->y + 1;
	while (curLine < height && drawHorizontalLine(canvas, curLine)) curLine++;
}
