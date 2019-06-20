// Mandelbrot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <complex>
#include "lodepng.h"

using namespace std;

using Complex = complex<double>;

unique_ptr<uint8_t[]> BuildMandelbrot(const Complex& from, const Complex& to, int width, int height);
uint8_t MandelbrotColor(const Complex& c);
bool SaveToBitmap(const char* filename, const uint8_t* image, int width, int height);

int main(int argc, const char* argv[]) {
	Complex from(-1.75, -1), to(.75, 1);

	auto width = 640, height = 480;
	auto bitmap = BuildMandelbrot(from, to, width, height);
	if (bitmap) {
		SaveToBitmap("image.png", bitmap.get(), width, height);
	}

	return 0;
}

unique_ptr<uint8_t[]> BuildMandelbrot(const Complex& from, const Complex& to, int width, int height) {
	auto bitmap = make_unique<uint8_t[]>((size_t)width * height);
	if (!bitmap)
		return nullptr;

	auto size = to - from;

	for(int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) {
			auto c = from + Complex(x * size.real() / width, y * size.imag() / height);
			auto color = MandelbrotColor(c);
			bitmap[x + (size_t)width * ((size_t)height - y - 1)] = color;
		}

	return bitmap;
}

uint8_t MandelbrotColor(const Complex& c) {
	const int iterations = 1000;
	Complex z = 0;
	for (int i = 0; i < iterations && abs(z) <= 2; i++) {
		z = z * z + c;
	}

	return abs(z) > 2 ? 0xff : 0;
}

bool SaveToBitmap(const char* filename, const uint8_t* image, int width, int height) {
	lodepng::State state;
	auto error = lodepng::encode(filename, image, width, height, LCT_GREY, 8);

	return error == 0;
}
