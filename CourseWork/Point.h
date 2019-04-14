#ifndef POINT
#define POINT



/*
	Вспомогательная структура для хранения координат точек
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