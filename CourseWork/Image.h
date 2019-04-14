#ifndef IMAGE
#define IMAGE



#include <vector>
#include "Color.h"
#include "Canvas.h"



using namespace std;


typedef struct {
	int 
		// �������� �� ������ �����
		offset, 

		// ������ ����
		size;
} HeaderField;


class InfoHeader {
public:
	static const HeaderField headerSize;
};


// ��������� ������ CORE
class H12 : InfoHeader {
public:
	static const HeaderField 
		// ������ ������ � ��������. ����������� ����� ������ �� ������. ���� � ������������� �� ���������������.
		width,

		//����� ����� �� ������, ���������� ��� ���������: ������ ������ � �������� (���������� �������� �����) � ������� ���������� ����� � ��������� �������� (���� �����). ������� �������� �� ���������������.
		height,

		//� BMP ��������� ������ �������� 1. ��� ���� ������������ � ������� � �������� Windows
		planes,

		//���������� ��� �� �������
		bitCount;
};


// �������������� ���� ��������� ������ 3
class H40 {
public:
	static const HeaderField
		// ������ ������ � ��������. ����������� ����� ������ �� ������. ���� � ������������� �� ���������������.
		width,

		// ����� ����� �� ������, ���������� ��� ���������: ������ ������ � �������� (���������� �������� �����) � ������� ���������� ����� � ��������� �������� (���� �����). ������� �������� �� ���������������.
		height,

		// � BMP ��������� ������ �������� 1. ��� ���� ������������ � ������� � �������� Windows
		planes,

		// ���������� ��� �� �������
		bitCount,
		
		// ��������� �� ������ �������� �������� (��������� ������ 0)
		compression,
		
		// ������ ���������� ������ � ������. ����� ���� �������� ���� �������� �������������� ��������� ��������.
		sizeImage,
		
		// ���������� �������� �� ���� �� ����������� � ���������
		xPelsPerMeter,
		yPelsPerMeter,
		
		// ������ ������� ������ � �������
		colorUsed,
		
		// ���������� ����� �� ������ ������� ������ �� ��������� ������������ (������� � ����)
		colorImportant;
};


// �������������� ���� ��������� ������ 4
class H108 {
public:
	static const HeaderField
		// ������� ����� ��� ���������� �������� �������: ������������� ��������, �������, ������ � �������� �����-������
		// �� ������������, ��������� �������� ���� compression ����� ����
		redMask,
		greenMask,
		blueMask,
		alphaMask;
};


// �������������� ���� ��������� ������ 5
class H124 {
	// ����� ����� ������ ��� ������� - �������� ��������� ��������� ������ 124 ����� � ��������� ��� �� ���������� ��������� ������� ������
};


class FileHeader {
public:
	static const HeaderField
		type,
		size,
		reserved1,
		reserved2,
		offsetBits;
};


class Mask16 {
public:
	static const int
		// ����� � ���������� �����
		/*
		maskAlpha	= 0xf000,
		maskRed		= 0x0f00,
		maskGreen	= 0x00f0,
		maskBlue	= 0x000f;
		*/

		// ����� � ���������
		maskAlpha = 0x0000,
		maskRed = 0x7c00,
		maskGreen = 0x03e0,
		maskBlue = 0x001f;
};


class Mask24 {
public:
	static const int
		maskRed		= 0xff0000,
		maskGreen	= 0x00ff00,
		maskBlue	= 0x0000ff,
		maskAlpha	= 0x000000;
};


class Mask32 {
public:
	static const int
		maskAlpha	= 0x00000000,
		maskRed		= 0x00ff0000,
		maskGreen	= 0x0000ff00,
		maskBlue	= 0x000000ff;
};


class ColorParams {
public:
	// ������������ ��������, ������� ����� ��������� �����
	int redLimit,
		greenLimit,
		blueLimit,
		alphaLimit;

	// ������������ �� �������� ������������ (������ �� ������� ������ � ������ Color)
	bool alphaEnabled = false;

	void fillColor(Color *color, int alpha, int red, int green, int blue);

	void fillColorFromRawInfo(Color *color, char *rawInfo);

	// ���������� ������ �� 4-� �������� int []{alpha, red, green, blue}
	int* extractRealColors(Color *color);
};


typedef struct {
	char error;
	const char *message;
} RequestResult;


class Image
{
private:
	char fileHeader[14];
	char infoHeader[124];
	char colorTable[256][4];
	Canvas canvas;
	bool filled = false, useColorTable;
	int infoHeaderSize, redMask, greenMask, blueMask, alphaMask;

	int readHeaderField(const HeaderField field, char *header);

	int readInfoHeaderField(const HeaderField field);

	int readFileHeaderField(const HeaderField field);

	void writeHeaderField(const HeaderField field, char *source, char *header);

	void writeFileHeaderField(const HeaderField field, char *source);

	void writeInfoHeaderField(const HeaderField field, char *source);

	/*
		������� � ������ result ������ �� content. ����� ���������, ��� ��� ������ ��������������� ����� 
		���, � ������� ������� �������� ���� ������ � ���������: ��������, 0x00111100 � 0x00100100 ��� 
		������ ������� ���������. � ������ ������ ��� �� ����� (�.�. ��� ����� ������� ����), �� ������ ����� ���������. 
		������� �������� source ����� �� ��������, ������ ������� ������� ���������� ���� (��� 
		����������� ���� ����� ��� ����� 2), ��������� �  ��������� ���� � ���� ����� ����� ������� � �������� ������.
		len ��������� �� ����� �������� ������ (�����, ���� ����� ������ ������ 4 ����, ����� �� ������������ ������ ������)
	*/
	void addContentByMask(char *result, const int mask, int content, int len);

	int getContentByMask(int from, int mask);

	/* 
		���������� ������������ ������� ����� � ��� ��������� �������� ��������, ������ �� 
		���������� ������� � ���� � ���������� �� ���� ��� ��������� �������� ������. ������ ������� 
		������ �������������� ���������, ����� ������� �� ���������� ������� ������. ����������� 
		�������� �� ��������� ������ �����������: ����� ��������� ��� ��������, ��������� ����� 
		�������� ������� ������ ������������� ����� ������ � ������ �����.
		---
		��������! ������������ ������� ������ ����� ���������� ������ �� �����, �.�. ��������������� 
		��������� ���������� ����� �������� � ������������� ���������� �����������.
	*/
	void updateDataWithAppPreferences();

	/*
		��������� ����� � ������������ � ����������� ���, ��������� ��� �������
	*/
	void updateMasks();

	/*
		��������� ������������ �������� ������ � ������������� �� ���������� �����
	*/
	void updateColorLimits();

	int calculateLineWidthInBytes();


	bool fillFileHeader(ifstream *input);

	bool fillInfoHeader(ifstream *input);

	bool fillColorTable(ifstream *input);

	bool fillCanvas(ifstream *input);


	void writeFileHeader(ofstream *out);

	void writeInfoHeader(ofstream *out);

	void writeCanvas(ofstream *out);

public:
	Image();
	~Image();

	// ������������ ��������, ������� ����� ��������� ����
	ColorParams *colorParams;

	// ��������� ��������� ������� �� ����� � ��������� filename
	RequestResult* readFrom(const char *filename);

	// �������� ������ � ���� filename
	RequestResult* writeTo(const char *filename);

	Canvas* getCanvas();
};



#endif


