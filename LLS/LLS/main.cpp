#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImfRgba.h>
#include <OpenEXR/ImfArray.h>
#include <omp.h>
#include<windows.h>
#include<time.h>
#include <iostream>
#include <algorithm>
using namespace std;

void readRgba2(const char fileName[])
{
	Imf::RgbaInputFile file("linear_07_000.exr");
	Imath::Box2i dw = file.dataWindow();
	int width = dw.max.x - dw.min.x + 1;
	int height = dw.max.y - dw.min.y + 1;
	cout << width << endl;
	cout << height << endl;
	Imf::Array2D<Imf::Rgba> pixels(10, width);
	int i = 0;
	while (dw.min.y <= dw.max.y)
	{
		i++;
		file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
		file.readPixels(dw.min.y, min(dw.min.y + 9, dw.max.y));// processPixels (pixels)
		dw.min.y += 10;
		//cout << hex << pixels[0][0].r.bits() << " " << hex << pixels[0][0].g.bits() << " " 
		//<< hex << pixels[0][0].b.bits() << " " << hex << pixels[0][0].a.bits() << endl;
	}
	cout << i << endl;
}

void
readRgba1(const char fileName[],
	Imf::Array2D<Imf::Rgba> &pixels,
	int &width,
	int &height)
{
	Imf::RgbaInputFile file(fileName);
	Imath::Box2i dw = file.dataWindow();
	width = dw.max.x - dw.min.x + 1;
	height = dw.max.y - dw.min.y + 1;
	pixels.resizeErase(height, width);
	file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
	file.readPixels(dw.min.y, dw.max.y);
}

int main() {
	Imf::setGlobalThreadCount(omp_get_num_procs());
	cout << omp_get_num_procs() << endl;
	Imf::Array2D<Imf::Rgba> pixels(2000, 2000);
	int w = 2000;
	int h = 2000;

	LARGE_INTEGER start, end, frequency;
	double time = { 0 };
	QueryPerformanceCounter(&start);

	//readRgba2("linear_07_000.exr");
	readRgba1("linear_07_000.exr", pixels, w, h);

	QueryPerformanceCounter(&end);
	if (!QueryPerformanceFrequency(&frequency))
		return -1;
	time = (double)(end.QuadPart - start.QuadPart) / (double)frequency.QuadPart;
	cout << "time: " << time << endl;
}