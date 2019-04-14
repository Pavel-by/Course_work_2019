#include "Line.h"
#include "Constants.h"
#include "Circle.h"



Line::Line() {
	init();
}


Line::Line(Point *p1, Point *p2)
{
	init();
	set(p1, p2);
}


Line::~Line() {
	delete line, rect;
}


void Line::init() {
	line = new MathLine;
	rect = new Rect;
}


void Line::set(Point *p1, Point *p2) {
	if (p1->x < p2->x) {
		this->p1 = p1;
		this->p2 = p2;
	}
	else {
		this->p1 = p2;
		this->p2 = p1;
	}
	line->set(p1, p2);
	rect->set(p1, p2);
}


int Line::drawLine(Canvas *canvas, int lineIndex) {
	int changedCount = 0,
		pixelWidth = canvas->getWidth();
	Point *startPoint = new Point((int)line->calcX(lineIndex), lineIndex),
		*curPoint = new Point,
		*crossPoint;
	Color *tmpColor = new Color(color);
	float distance;
	curPoint->set(startPoint->x, startPoint->y);
	while (curPoint->x < pixelWidth) {
		tmpColor->a = color->a;
		crossPoint = line->calculateCrossPoint(curPoint);
		if (crossPoint == nullptr) break;
		if (crossPoint->x < rect->left || crossPoint->x > rect->right
			|| crossPoint->y < rect->top || crossPoint->y > rect->bottom) {
			curPoint->x++;
			continue;
		}
		distance = crossPoint->distance(curPoint);
		if (distance - ANTIALIAS >= this->width) break;
		if (distance > this->width) tmpColor->a *= ANTIALIAS - (distance - this->width);
		canvas->get(curPoint->x, curPoint->y)->overlap(tmpColor);
		changedCount++;
		curPoint->x++;
	}
	curPoint->set(startPoint->x - 1, startPoint->y);
	while (curPoint->x >= 0) {
		tmpColor->a = color->a;
		crossPoint = line->calculateCrossPoint(curPoint);
		if (crossPoint == nullptr) break;
		if (crossPoint->x < p1->x || crossPoint->x > p2->x
			|| crossPoint->y < rect->top || crossPoint->y > rect->bottom) {
			curPoint->x--;
			continue;
		}
		distance = crossPoint->distance(curPoint);
		if (distance - ANTIALIAS >= this->width) break;
		if (distance > this->width) tmpColor->a *= ANTIALIAS - (distance - this->width);
		canvas->get(curPoint->x, curPoint->y)->overlap(tmpColor);
		changedCount++;
		curPoint->x--;
	}
	delete startPoint, curPoint, tmpColor;
	return changedCount;
}


void Line::drawSharp(Canvas *canvas) {
	int line = p1->y,
		imageHeight = canvas->getHeight();
	while (line < imageHeight && drawLine(canvas, line)) line++;
	line = p1->y - 1;
	while (line >= 0 && drawLine(canvas, line)) line--;
}


void Line::draw(Canvas *canvas) {
	drawSharp(canvas);
	Circle *circle = new Circle(p1, width, color);
	circle->draw(canvas);
	circle->center = p2;
	circle->draw(canvas);
	delete circle;
}