#ifndef POINT
#define POINT



/*
	��������������� ��������� ��� �������� ��������� �����
*/
class Point {
public:
	float x, y;

	Point() {};

	Point(float x, float y);

	void set(float x, float y);

	float distance(Point *p);

};



#endif