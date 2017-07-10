#include <ImfRgbaFile.h>
#include <ImathBox.h>
#include <ImathVec.h>
#include <ImfRgba.h>
#include <ImfArray.h>
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

/*void main() {
	readRgba2("F:\\workplace\\LLS");
}*/