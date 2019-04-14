#include "TriangleFractal.h"
#include "Line.h"



TriangleFractal::TriangleFractal()
{}


TriangleFractal::TriangleFractal(Point *p1, Point *p2, Point *p3) {
	set(p1, p2, p3);
}


void TriangleFractal::set(Point *p1, Point *p2, Point *p3) {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}


void TriangleFractal::drawInnerLines(Canvas *canvas) {
	float minimumDistance = lineWidth * 16;
	if (lineWidth < 1
		|| minimumDistance > p1->distance(p2)
		|| minimumDistance > p2->distance(p3)
		|| minimumDistance > p3->distance(p1)
		) return;

	Line line;
	line.width = lineWidth;
	line.color = color;

	TriangleFractal tmp;
	tmp.color = color;
	tmp.lineWidth = lineWidth;
	Point tp1, tp2, tp3;

	tp1.set(p1->x, p1->y);
	tp2.set((p1->x + p2->x) / 2, (p1->y + p2->y) / 2);
	tp3.set((p1->x + p3->x) / 2, (p1->y + p3->y) / 2);
	line.set(&tp2, &tp3); line.draw(canvas);
	tmp.set(&tp1, &tp2, &tp3); tmp.drawInnerLines(canvas);

	tp1.set(p2->x, p2->y);
	tp2.set((p2->x + p1->x) / 2, (p2->y + p1->y) / 2);
	tp3.set((p2->x + p3->x) / 2, (p2->y + p3->y) / 2);
	line.set(&tp2, &tp3); line.draw(canvas);
	tmp.set(&tp1, &tp2, &tp3); tmp.drawInnerLines(canvas);

	tp1.set(p3->x, p3->y);
	tp2.set((p3->x + p2->x) / 2, (p3->y + p2->y) / 2);
	tp3.set((p3->x + p1->x) / 2, (p3->y + p1->y) / 2);
	line.set(&tp2, &tp3); line.draw(canvas);
	tmp.set(&tp1, &tp2, &tp3); tmp.drawInnerLines(canvas);
}


void TriangleFractal::draw(Canvas *canvas) {
	Line line;
	line.width = lineWidth;
	line.color = color;

	line.set(p1, p2);
	line.draw(canvas);
	line.set(p1, p3);
	line.draw(canvas);
	line.set(p2, p3);
	line.draw(canvas);
	drawInnerLines(canvas);
}


TriangleFractal::~TriangleFractal()
{}
