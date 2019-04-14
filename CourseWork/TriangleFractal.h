#ifndef TRIANGLE_FRACTAL
#define TRIANGLE_FRACTAL



#include "Color.h"
#include "Point.h"
#include "Canvas.h"



class TriangleFractal
{
private:

	/* 
		����� �������� ���������� ����� � ��� �� ����� �������� �������� ���: ������� ������ ����� (����� 
		draw), ����� ���� ����� �������� drawInnerLines, ������� ������ 3 ���������� ����� (����������� 
		������� ���������� �������������) � ���������� �������� ��������� ���������� ����� � ������������ 
		�������������
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
