#ifndef RECT
#define RECT



#include "Point.h"



class Rect
{
public:
	float top, left, right, bottom;

	Rect();

	Rect(Point *p1, Point *p2);

	Rect(float left, float top, float right, float bottom);

	void set(Point *p1, Point *p2);

	~Rect();
};



#endif