#include "MathLine.h"
#include "Constants.h"
#include <math.h>



MathLine::MathLine() {
	init();
}


MathLine::MathLine(Point *p1, Point *p2) {
	set(p1, p2); 
	init();
}


MathLine::~MathLine() {
	delete directVector, normalVector;
}


void MathLine::init() {
	directVector = new Point;
	normalVector = new Point;
}


int MathLine::eq(float n, float m) {
	float t = n - m;
	return -NUMBER_ACCURACY < t && t < NUMBER_ACCURACY;
}


int MathLine::isSinglePoint() {
	return eq(p1->x, p2->x) && eq(p1->y, p2->y);
}


void MathLine::set(Point *p1, Point *p2) {
	this->p1 = p1;
	this->p2 = p2;
	if (isSinglePoint()) return;
	if (eq(p1->x, p2->x)) {
		directVector->set(1, 0);
		normalVector->set(0, 1);
	}
	else if (eq(p1->y, p2->y)) {
		directVector->set(0, 1);
		normalVector->set(1, 0);
	}
	else {
		directVector->set(p2->x - p1->x, p2->y - p1->y);
		normalVector->set(-(directVector->y / directVector->x), 1);
		k = directVector->y / directVector->x;
		b = p1->y - k * p1->x;
	}
	// Дальше через нормальное уравнение прямой на плоскости
	float tA = abs(-b / k),
		tB = abs(b),
		tC = sqrt(tA*tA + tB * tB);
	startDistance = tA * tB / tC;
	sinA = tA / tC;
	cosA = tB / tC;
	//Дальше расчитываем знаки синуса и косинуса
	if (b > 0 && k > 0)  // 2 четверть
		cosA = -cosA;
	else if (b < 0 && k < 0) { // 3 четверть
		cosA = -cosA;
		sinA = -sinA;
	}
	else if (b < 0 && k > 0) // 4 четверть
		sinA = -sinA;
}


float MathLine::distanceTo(Point *point) {
	return abs(point->x*cosA + point->y*sinA - startDistance);
}


Point* MathLine::calculateCrossPoint(Point *p) {
	if (isSinglePoint()) return NULL;
	if (eq(p1->x, p2->x)) return new Point(p1->x, p->y);
	if (eq(p1->y, p2->y)) return new Point(p->x, p1->y);
	Point *result = new Point;
	float k2 = normalVector->y / normalVector->x,
		b2 = p->y - k2 * p->x;
	result->x = (b2 - b) / (k - k2);
	result->y = k * result->x + b;
	return result;
}


float MathLine::calcX(float y) {
	if (isSinglePoint()) return 0;
	if (eq(p1->x, p2->x)) return p1->x;
	if (eq(p1->y, p2->y)) return 0;
	return (y - b) / k;
}


float MathLine::calcY(float x) {
	if (isSinglePoint()) return 0;
	if (eq(p1->x, p2->x)) return 0;
	if (eq(p1->y, p2->y)) return p1->y;
	return (x * k) + b;
}
