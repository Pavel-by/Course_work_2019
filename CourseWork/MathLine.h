#ifndef MATH_LINE
#define MATH_LINE



#include "Point.h"



class MathLine
{
private:
	Point *p1, *p2,
		*directVector,
		*normalVector;
	// ������������ � ��������� y = kx + b
	float k, b;

	// ���������� �� ������ ���������, ����� � ������� ���� (�� ����������� ��������� ������ �� ���������)
	// PS: d = |x0cosA + y0sinA - startDistance| - ���������� �� ������
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
		��������� ����� ����������� �������� ������ � ������, ���������������� 
		������ � ���������� ����� ����� linePoint

		PS: �� ��������������, �.�. ����� ��������
	*/
	Point* calculateCrossPoint(Point *linePoint);

	float calcX(float y);
	float calcY(float x);

	/*
		���������� �� �����	
	*/
	float distanceTo(Point *p);
};



#endif