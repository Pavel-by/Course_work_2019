#ifndef TRIANGLE_FRACTAL
#define TRIANGLE_FRACTAL



#include "Color.h"
#include "Point.h"
#include "Canvas.h"



class TriangleFractal
{
private:

	/* 
		Чтобы избежать прорисовки одних и тех же линий алгоритм работает так: сначала рисует рамку (метод 
		draw), затем этот метод вызывает drawInnerLines, который рисует 3 внутренние линии (заканчивает 
		границы внутренних треугольников) и рекурсивно вызывает отрисовку внутренних линий у получившихся 
		треугольников
	*/
	void drawInnerLines(Canvas *canvas);

public:

	Point *p1, *p2, *p3;
	Color *color;
	float lineWidth = 0;

	TriangleFractal();
	TriangleFractal(Point *p1, Point *p2, Point *p3);

	void set(Point *p1, Point *p2, Point *p3);

	void draw(Canvas *canvas);

	~TriangleFractal();
};



#endif
