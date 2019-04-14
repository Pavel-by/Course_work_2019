#ifndef VECTOR
#define VECTOR



class Vector {
public:

	float x, y;

	Vector();
	Vector(float x, float y);

	void set(float x, float y);

	Vector* calculateNormal();
};



#endif