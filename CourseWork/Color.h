#ifndef COLOR
#define COLOR



class Color {
public:

	Color() {};
	Color(Color *from);
	Color(float a, float r, float g, float b);

	// Значения красного, зеленого и синего цвето
	float r, g, b, a;

	// Наложить новый цвет
	void overlap(Color *newColor);

	// Скопировать значения
	void from(Color *color);
};



#endif