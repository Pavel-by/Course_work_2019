#ifndef CANVAS
#define CANVAS



#include "Color.h"



class Canvas
{
private:

	Color *canvas;
	int width = 0, height = 0;


public:

	Canvas();

	Canvas(int width, int height);

	void resize(int width, int height);

	int getWidth();
	int getHeight();

	void fill(Color *color);

	Color* get(int x, int y);

	~Canvas();
};



#endif