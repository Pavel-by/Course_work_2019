#include "Rect.h"



Rect::Rect()
{
}


Rect::Rect(Point *p1, Point *p2) {
	set(p1, p2);
}


Rect::Rect(float left, float top, float right, float bottom) {
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}


void Rect::set(Point *p1, Point *p2) {
	if (p1->x > p2->x) {
		left = p2->x;
		right = p1->x;
	}
	else {
		left = p1->x;
		right = p2->x;
	}
	if (p1->y > p2->y) {
		top = p2->y;
		bottom = p1->y;
	}
	else {
		top = p1->y;
		bottom = p2->y;
	}
}


Rect::~Rect()
{
}
