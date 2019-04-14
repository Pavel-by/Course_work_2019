#ifndef CIRCLE
#define CIRCLE



#include "Color.h"
#include "Canvas.h"
#include "Point.h"



class Circle
{
private:
	int drawHorizontalLine(Canvas* canvas, int lineIndex);

public:
	int radius;
	Color *color;
	Point *center;
	Circle(Point *center);
	Circle(Point *center, int radius);
	Circle(Point *center, int radius, Color *color);
	void draw(Canvas *canvas);
};



#endif