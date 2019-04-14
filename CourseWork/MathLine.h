#ifndef MATH_LINE
#define MATH_LINE



#include "Point.h"



class MathLine
{
private:
	Point *p1, *p2,
		*directVector,
		*normalVector;
	// Коэффициенты в уравнении y = kx + b
	float k, b;

	// Расстояние до начала координат, синус и косинус угла (из нормального уравнения прямой на плоскости)
	// PS: d = |x0cosA + y0sinA - startDistance| - расстояние до прямой
	float startDistance, sinA, cosA;

	int eq(float n, float m);

	int isSinglePoint();

	void init();

public:
	MathLine();
	MathLine(Point *p1, Point *p2);
	~MathLine();

	void set(Point *p1, Point *p2);

	/* 
		Расчитать точку пересечения заданной прямой и прямой, перпендикулярной 
		данной и проходящей через точку linePoint

		PS: Не рекоммендуется, т.к. очень затратно
	*/
	Point* calculateCrossPoint(Point *linePoint);

	float calcX(float y);
	float calcY(float x);

	/*
		Расстояние до точки	
	*/
	float distanceTo(Point *p);
};



#endif