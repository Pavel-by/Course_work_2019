#include <iostream>
#include <fstream>
#include "image.h"
#include "Circle.h"
#include "Line.h"
#include "TriangleFractal.h"

using namespace std;

int main() {
	Image image;
	if (image.readFrom("test_big.bmp")) {
		cout << "Success read!" << endl;
		Point a = { 100, 100 };
		Point *p1 = new Point(30, 30), 
			*p2 = new Point(500, 30),
			*p3 = new Point(240, 200);
		TriangleFractal *fractal = new TriangleFractal(p1, p2, p3);
		fractal->color = new Color(1, 1, 0, 0);
		fractal->lineWidth = 1;
		fractal->draw(image.getCanvas());
		if (image.writeTo("result.bmp")) cout << "Success write" << endl;
		else cout << "Failed write" << endl;
	}
	else cout << "Error read" << endl;
	cin.get();
}