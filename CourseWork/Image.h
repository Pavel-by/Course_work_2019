#ifndef IMAGE
#define IMAGE



#include <vector>
#include "Color.h"
#include "Canvas.h"



using namespace std;


typedef struct {
	int 
		// Смешение от начала файла
		offset, 

		// Размер поля
		size;
} HeaderField;


class InfoHeader {
public:
	static const HeaderField headerSize;
};


// Заголовок версии CORE
class H12 : InfoHeader {
public:
	static const HeaderField 
		// Ширина растра в пикселях. Указывается целым числом со знаком. Ноль и отрицательные не документированы.
		width,

		//Целое число со знаком, содержащее два параметра: высота растра в пикселях (абсолютное значение числа) и порядок следования строк в двумерных массивах (знак числа). Нулевое значение не документировано.
		height,

		//В BMP допустимо только значение 1. Это поле используется в значках и курсорах Windows
		planes,

		//Количество бит на пиксель
		bitCount;
};


// Дополнительные поля заголовка версии 3
class H40 {
public:
	static const HeaderField
		// Ширина растра в пикселях. Указывается целым числом со знаком. Ноль и отрицательные не документированы.
		width,

		// Целое число со знаком, содержащее два параметра: высота растра в пикселях (абсолютное значение числа) и порядок следования строк в двумерных массивах (знак числа). Нулевое значение не документировано.
		height,

		// В BMP допустимо только значение 1. Это поле используется в значках и курсорах Windows
		planes,

		// Количество бит на пиксель
		bitCount,
		
		// Указывает на способ хранения пикселей (допустимо только 0)
		compression,
		
		// Размер пиксельных данных в байтах. Может быть обнулено если хранение осуществляется двумерным массивом.
		sizeImage,
		
		// Количество пикселей на метр по горизонтали и вертикали
		xPelsPerMeter,
		yPelsPerMeter,
		
		// Размер таблицы цветов в ячейках
		colorUsed,
		
		// Количество ячеек от начала таблицы цветов до последней используемой (включая её саму)
		colorImportant;
};


// Дополнительные поля заголовка версии 4
class H108 {
public:
	static const HeaderField
		// Битовые маски для извлечения значений каналов: интенсивность красного, зелёного, синего и значение альфа-канала
		// Не используются, поскольку значение поля compression равно нулю
		redMask,
		greenMask,
		blueMask,
		alphaMask;
};


// Дополнительные поля заголовка версии 5
class H124 {
	// Класс нужен просто для красоты - неудобно считывать заголовок дляной 124 байта и проверять его по параметрам заголовка прошлой версии
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
		// Маски с рандомного сайта
		/*
		maskAlpha	= 0xf000,
		maskRed		= 0x0f00,
		maskGreen	= 0x00f0,
		maskBlue	= 0x000f;
		*/

		// Маски с Википедии
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
	// Максимальные значения, которые могут принимать цвета
	int redLimit,
		greenLimit,
		blueLimit,
		alphaLimit;

	// Используется ли параметр прозрачности (влияет на слияние цветов в классе Color)
	bool alphaEnabled = false;

	void fillColor(Color *color, int alpha, int red, int green, int blue);

	void fillColorFromRawInfo(Color *color, char *rawInfo);

	// Возвращает массив из 4-х значений int []{alpha, red, green, blue}
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
		Заносит в строку result данные из content. Нужно учитывать, что под маской подразумевается набор 
		бит, в котором важными являются лишь первый и последний: например, 0x00111100 и 0x00100100 для 
		данной функции идентичны. В данном случае это не важно (т.к. все маски первого вида), но вообще стоит учитывать. 
		Функция сдвигает source влево на величину, равную позиции первого ненулевого бита (для 
		приведенных выше масок это будет 2), применяет к  побитовую конь и лишь после этого заносит в конечную строку.
		len указывает на длины конечной строки (важно, если длина строки меньше 4 байт, чтобы не перезаписать лишние данные)
	*/
	void addContentByMask(char *result, const int mask, int content, int len);

	int getContentByMask(int from, int mask);

	/* 
		Приложение поддерживает входные файлы с уже имеющейся цветовой таблицей, однако не 
		записывает таблицу в файл и использует ее лишь для получения значений цветов. Данная функция 
		меняет информационный заголовок, чтобы указать на отсутствие таблицы цветов. Аналогичная 
		ситуация со смещением данных изображения: нужно сократить его значение, поскольку после 
		удаления таблицы цветов освобождается цасть памяти в начале файла.
		---
		Внимание! Использовать функцию только после считывания данных из файла, т.к. преждевременное 
		изменение заголовков может привести к неправильному считыванию изображения.
	*/
	void updateDataWithAppPreferences();

	/*
		Обновляет маски в соответствии с количеством бит, отводимых под пиксель
	*/
	void updateMasks();

	/*
		Обновляет максимальные значения цветов в соответствием со значениями масок
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

	// Максимальное значение, которое может принимать цвет
	ColorParams *colorParams;

	// Заполнить структуру данными из файла с названием filename
	RequestResult* readFrom(const char *filename);

	// Записать данные в файл filename
	RequestResult* writeTo(const char *filename);

	Canvas* getCanvas();
};



#endif


