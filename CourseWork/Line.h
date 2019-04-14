#ifndef LINE
#define LINE



#include "Point.h"
#include "MathLine.h"
#include "Rect.h"
#include "Canvas.h"



class Line
{
private:

	Point *p1, *p2;
	Rect *rect;
	MathLine *line;

	void init();

	int drawLine(Canvas *canvas, int lineIndex);

	Point directV, normalV;


public:

	int width;
	Color *color;

	Line();
	Line(Point *p1, Point *p2);
	~Line();

	void set(Point *p1, Point *p2);

	void draw(Canvas *canvas);

	void drawSharp(Canvas *canvas);
};



#endif