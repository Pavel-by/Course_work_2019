#define _CRT_SECURE_NO_WARNINGS


#include "image.h"
#include <fstream>
#include <vector>
#include <math.h>
#include <string.h>
#include "common.h"


#include <iostream>


using namespace std;

const HeaderField FileHeader::type = { 0x0, 4 };
const HeaderField FileHeader::size = { 0x2, 4 };
const HeaderField FileHeader::reserved1 = { 0x6, 2 };
const HeaderField FileHeader::reserved2 = { 0x8, 2 };
const HeaderField FileHeader::offsetBits = { 0xa, 4 };

const HeaderField InfoHeader::headerSize = { 0x00, 4 };

const HeaderField H12::width = { 0x04, 2 };
const HeaderField H12::height = { 0x06, 2 };
const HeaderField H12::planes = { 0x08, 2 };
const HeaderField H12::bitCount = { 0x0A, 2 };

const HeaderField H40::width = { 0x04, 4 };
const HeaderField H40::height = { 0x08, 4 };
const HeaderField H40::planes = { 0x0c, 2 };
const HeaderField H40::bitCount = { 0x0e, 2 };
const HeaderField H40::compression = { 0x10, 4 };
const HeaderField H40::sizeImage = { 0x14, 4 };
const HeaderField H40::xPelsPerMeter = { 0x18, 4 };
const HeaderField H40::yPelsPerMeter = { 0x1c, 4 };
const HeaderField H40::colorUsed = { 0x20, 4 };
const HeaderField H40::colorImportant = { 0x24, 4 };

const HeaderField H108::redMask = { 0x28, 4 };
const HeaderField H108::greenMask = { 0x2c, 4 };
const HeaderField H108::blueMask = { 0x30, 4 };
const HeaderField H108::alphaMask = { 0x34, 4 };



Image::Image() {
	colorParams = new ColorParams;
}

Image::~Image() {
	free(colorParams);
}


int Image::readHeaderField(const HeaderField field, char *source) {
	return pullIntValue(source + field.offset, field.size);
}


int Image::readInfoHeaderField(const HeaderField field) {
	return readHeaderField(field, infoHeader);
}


int Image::readFileHeaderField(const HeaderField field) {
	return readHeaderField(field, fileHeader);
}


void Image::writeHeaderField(const HeaderField field, char *source, char *header) {
	strcpy_force(header + field.offset, source, field.size);
}


void Image::writeFileHeaderField(const HeaderField field, char *source) {
	writeHeaderField(field, source, fileHeader);
}


void Image::writeInfoHeaderField(const HeaderField field, char *source) {
	writeHeaderField(field, source, infoHeader);
}


int Image::calculateLineWidthInBytes() {
	long int result;
	int width = readInfoHeaderField(H40::width),
		bitCount = readInfoHeaderField(H40::bitCount);
	result = width;
	result = ((result * bitCount + 31) / 32 ) * 4;
	return result;
}


bool Image::fillFileHeader(ifstream *input) {
	cout << "Fill file header" << endl;
	input->seekg(0);
	input->read(fileHeader, 12);
	return true;
}


bool Image::fillInfoHeader(ifstream *input) {
	cout << "Fill info header" << endl;
	input->seekg(14);
	input->read((char*)&infoHeaderSize, InfoHeader::headerSize.size);
	if (infoHeaderSize != 40 && infoHeaderSize != 108 && infoHeaderSize != 124)
		return false;
	input->seekg(14);
	input->read(infoHeader, infoHeaderSize);
	return true;
}


bool Image::fillColorTable(ifstream *input) {
	cout << "Fill color table" << endl;
	short int bitCount, colorsUsed;
	bitCount = readInfoHeaderField(H40::bitCount);
	colorsUsed = readInfoHeaderField(H40::colorUsed);
	if (bitCount > 8 && (infoHeaderSize == 12 || !colorsUsed)) {
		useColorTable = false;
		return true;
	}
	useColorTable = true;
	int colorsCount = colorsUsed;
	if (infoHeaderSize == 12 || colorsUsed == 0) {
		colorsCount = 1 << bitCount;
	}
	input->read((char*)colorTable, colorsCount * 4);
	return true;
}


/* 
Рассчитать размер элемента по маске. Используется для получения максимально допустимых значений цветов ARGB
*/
int calculateMaxSizeByMask(int mask) {
	if (mask == 0) return 0;
	while (!(mask & 1)) mask >>= 1;
	int result = 0b10;
	while ((mask >>= 1) & 1) result <<= 1;
	return result - 1;
}


int Image::getContentByMask(int from, int mask) {
	if (!mask) return 0;
	while (!(mask & 1)) {
		mask >>= 1;
		from >>= 1;
	}
	return mask & from;
}


bool Image::fillCanvas(ifstream *input) {
	int pixelHeight = readInfoHeaderField(H40::height),
		pixelWidth = readInfoHeaderField(H40::width),
		bitCount = readInfoHeaderField(H40::bitCount);
	canvas.resize(pixelWidth, pixelHeight);
	int lineWidth = calculateLineWidthInBytes();
	char *line = new char[lineWidth];
	input->seekg(readFileHeaderField(FileHeader::offsetBits));
	updateMasks();
	updateColorLimits();
	for (int rowCounter = 0; rowCounter < abs(pixelHeight); rowCounter++) {
		int 
			// Индекс текущей строки в конечном массиве изображения (canvas)
			row = pixelHeight < 0 ? rowCounter : pixelHeight - rowCounter - 1,
			// Индекс колонки в конечном массиве (canvas)
			column = 0,
			// Текущий индекс курсора в "сырой" строке (line)
			curSymbol = 0;
		input->read(line, lineWidth);
		while (column < pixelWidth) {
			switch (bitCount) {
			case 1:
			case 2:
			case 4:
			case 8: {
				int mask = 1, pixelsInByte = 8 / bitCount;
				for (int i = 1; i < bitCount; i++) mask = (mask << 1) | 1;
				for (int i = 0; i < pixelsInByte && column < pixelWidth; i++)
					colorParams->fillColorFromRawInfo(
						canvas.get(column++, row),
						colorTable[
							line[curSymbol] & (mask << ((pixelsInByte - 1 - i) * bitCount))
						]
					);
				curSymbol++;
				break;
			}
			default: {
				if (useColorTable) {
					colorParams->fillColorFromRawInfo(
						canvas.get(column++, row),
						colorTable[
							pullIntValue(line + curSymbol, bitCount / 8)
						]
					);						
				}
				else {
					int pulledValue = pullIntValue(line + curSymbol, bitCount / 8);
					colorParams->fillColor(
						canvas.get(column, row),
						getContentByMask(pulledValue, alphaMask),
						getContentByMask(pulledValue, redMask),
						getContentByMask(pulledValue, greenMask),
						getContentByMask(pulledValue, blueMask)
					);
					column++; curSymbol += bitCount / 8;
				}
			}
			}
		}
	}
	free(line);
	return true;
}


void Image::updateMasks() {
	switch (readInfoHeaderField(H40::bitCount)) {
	case 16: {
		redMask = Mask16::maskRed;
		blueMask = Mask16::maskBlue;
		greenMask = Mask16::maskGreen;
		alphaMask = Mask16::maskAlpha;
		break;
	}
	case 24: {
		redMask = Mask24::maskRed;
		blueMask = Mask24::maskBlue;
		greenMask = Mask24::maskGreen;
		alphaMask = Mask24::maskAlpha;
		break;
	}
	case 32: {
		redMask = Mask32::maskRed;
		blueMask = Mask32::maskBlue;
		greenMask = Mask32::maskGreen;
		alphaMask = Mask32::maskAlpha;
		break;
	}
	}
}


void Image::updateColorLimits() {
	colorParams->redLimit = calculateMaxSizeByMask(redMask);
	colorParams->blueLimit = calculateMaxSizeByMask(blueMask);
	colorParams->greenLimit = calculateMaxSizeByMask(greenMask);
	colorParams->alphaLimit = calculateMaxSizeByMask(alphaMask);
}


void Image::updateDataWithAppPreferences() {
	int tmp = 0;
	writeInfoHeaderField(H40::colorUsed, (char*)&tmp);
	writeInfoHeaderField(H40::colorImportant, (char*)&tmp);
	tmp = sizeof(fileHeader) + infoHeaderSize;
	writeFileHeaderField(FileHeader::offsetBits, (char*)&tmp);
	if (readInfoHeaderField(H40::bitCount) <= 8 && useColorTable) {
		tmp = 32;
		writeInfoHeaderField(H40::bitCount, (char*)&tmp);
		updateMasks();
		updateColorLimits();
	}
	if ((tmp = readInfoHeaderField(H40::height)) < 0) {
		writeInfoHeaderField(H40::height, (char*)&tmp);
	}
}


RequestResult* Image::readFrom(const char *filename) {
	RequestResult *result = new RequestResult{ 0, NULL };
	char temp[4];
	ifstream input;
	try {
		input.open(filename, ios::binary);
		if (!fillFileHeader(&input) || !fillInfoHeader(&input)) {
			result->error = 1;
			result->message = "Данный формат заголовков не поддерживается";
		}
		if (!result->error) fillColorTable(&input);
		if (!result->error && !fillCanvas(&input)) {
			result->error = 1;
			result->message = "Не удалось считать данные изображения";
		} 
		if (!result->error) updateDataWithAppPreferences();
	}
	catch (exception e) {
		result->error = 1;
		result->message = "Неизвестная ошибка в процессе выполнения";
	}
	if (input.is_open()) input.close();
	filled = !result->error;
	cout << "End read" << endl;
	return result;	
}


void Image::writeFileHeader(ofstream *out) {
	out->write(fileHeader, sizeof(fileHeader));
}


void Image::writeInfoHeader(ofstream *out) {
	out->write(infoHeader, infoHeaderSize);
}


void Image::addContentByMask(char *result, const int mask, int content, int len) {
	if (!mask) return;
	int tmp = 0, offsetCounter = 1;
	strcpy_force((char*)&tmp, result, len);
	while (!(mask & offsetCounter)) {
		offsetCounter <<= 1;
		content <<= 1;
	}
	content &= mask;
	tmp &= ~mask;
	tmp |= content;
	strcpy_force(result, (char*)&tmp, len);
}


void Image::writeCanvas(ofstream *out) {
	int bitCount = readInfoHeaderField(H40::bitCount),
		height = readInfoHeaderField(H40::height),
		width = readInfoHeaderField(H40::width),
		lineWidth = calculateLineWidthInBytes();
	char *line = new char[lineWidth];
	for (int lineIndex = height - 1; lineIndex >= 0; lineIndex--) {
		for (int i = 0; i < lineWidth; i++) line[i] = 0;
		int curSymbol = 0;
		for (int column = 0; column < width; column++) {
			int *colors = colorParams->extractRealColors(canvas.get(column, lineIndex));
			addContentByMask(line + curSymbol, redMask, colors[1], bitCount / 8);
			addContentByMask(line + curSymbol, greenMask, colors[2], bitCount / 8);
			addContentByMask(line + curSymbol, blueMask, colors[3], bitCount / 8);
			addContentByMask(line + curSymbol, alphaMask, colors[0], bitCount / 8);
			curSymbol += bitCount / 8;
		}
		out->write(line, lineWidth);
	}
}


RequestResult *Image::writeTo(const char *filename) {
	RequestResult *result = new RequestResult{ 0, "" };
	ofstream out;
	try {
		out.open(filename, ios::binary | ofstream::trunc);
		out.seekp(0);
		writeFileHeader(&out);
		writeInfoHeader(&out);
		writeCanvas(&out);
	}
	catch (ios_base::failure e) {
		result->error = 1;
		result->message = "Неизвестная ошибка в процессе записи в файл";
	}
	if (out.is_open()) out.close();
	return result;
}


void ColorParams::fillColor(Color *color, int alpha, int red, int green, int blue) {
	color->a = (float)alpha / alphaLimit;
	color->r = (float)red / redLimit;
	color->g = (float)green / greenLimit;
	color->b = (float)blue / blueLimit;
}


void ColorParams::fillColorFromRawInfo(Color *color, char *rawInfo) {
	color->b = (float)rawInfo[0] / blueLimit;
	color->g = (float)rawInfo[1] / greenLimit;
	color->r = (float)rawInfo[2] / redLimit;
	color->a = 1;
}


int* ColorParams::extractRealColors(Color *color) {
	return new int[4] {
		(int) (color->a * alphaLimit),
		(int) (color->r * redLimit),
		(int) (color->g * greenLimit),
		(int) (color->b * blueLimit)
	};
}


Canvas *Image::getCanvas() {
	return &canvas;
}