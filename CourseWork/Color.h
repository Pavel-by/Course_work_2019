#ifndef COLOR
#define COLOR



class Color {
public:

	Color() {};
	Color(Color *from);
	Color(float a, float r, float g, float b);

	// �������� ��������, �������� � ������ �����
	float r, g, b, a;

	// �������� ����� ����
	void overlap(Color *newColor);

	// ����������� ��������
	void from(Color *color);
};



#endif