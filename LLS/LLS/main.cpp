#include <ImathBox.h>
#include <ImathVec.h>
#include <ImfRgbaFile.h>
#include <ImfRgba.h>
#include <ImfArray.h>

#include <omp.h>
#include <time.h>
#include <windows.h>
#include <io.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

void listFiles(const char * dir, vector<string>& files)
{
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
	if (handle == -1)
	{
		cout << "Failed to find first file!\n";
		return;
	}

	do
	{
		if (findData.attrib & _A_SUBDIR)
			continue;
		else
			files.push_back(findData.name);
	} while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
	_findclose(handle);    // 关闭搜索句柄
}

void readPixel(const char fileName[], vector<Imf::Rgba>& data) {
	Imf::RgbaInputFile file(fileName);
	Imath::Box2i dw = file.dataWindow();
	int width = dw.max.x - dw.min.x + 1;
	int height = dw.max.y - dw.min.y + 1;
	Imf::Array2D<Imf::Rgba> pixels(1, width);
	int x = 1976, y = 316;
	dw.min.y += y;
	file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
	file.readPixels(dw.min.y);
	data.push_back(pixels[0][x]);
}

void readFull(const char fileName[], Imf::Array2D<Imf::Rgba> &pixels, int &width, int &height) {
	Imf::RgbaInputFile file(fileName);
	Imath::Box2i dw = file.dataWindow();
	width = dw.max.x - dw.min.x + 1;
	height = dw.max.y - dw.min.y + 1;
	cout << width << endl;
	cout << height << endl;
	pixels.resizeErase(height, width);
	file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
	file.readPixels(dw.min.y, dw.max.y);

	ofstream outFile;
	outFile.open("data.csv", ios::out); // 打开模式可省略
	outFile << "y,x,r,g,b,a" << endl;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
		cout << i << " " << j << endl;
			outFile << i << ',' << j << ',' << pixels[i][j].r << ',' << pixels[i][j].g << ','
				<< pixels[i][j].b << ',' << pixels[i][j].a << endl;
		}
	}
	outFile.close();
}

int main() {
	Imf::setGlobalThreadCount(omp_get_num_procs());
	LARGE_INTEGER start, end, frequency;
	double time = { 0 };

	char dir[200] = "F:\\data\\scan\\*.*";
	vector<string> files;
	listFiles(dir, files);
	vector<Imf::Rgba> data;

	QueryPerformanceCounter(&start);
	int size = files.size();
	int i;
#pragma omp parallel for
	for (i = 0;i < size;i++)
	{
		readPixel(("f:\\data\\scan\\" + files[i]).c_str(), data);
	}

	//readPixel("F:\\data\\scan\\linear_07_050.exr", data);

	QueryPerformanceCounter(&end);
	if (!QueryPerformanceFrequency(&frequency))
		return -1;
	time = (double)(end.QuadPart - start.QuadPart) / (double)frequency.QuadPart;
	cout << "time: " << time << endl;

	ofstream outFile;
	outFile.open("data.csv", ios::out);
	outFile << "file,r,g,b,a" << endl;
	for (int i = 0; i < size; i++) {
		outFile << files[i] << ',' << data[i].r << ',' << data[i].g << ','
			<< data[i].b << ',' << data[i].a << endl;
	}
	outFile.close();

	return 0;
}