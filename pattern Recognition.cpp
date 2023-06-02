#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
using namespace std;
typedef int int32_t;
typedef short int16_t;
#pragma pack(push)
#pragma pack(1)
class BITMAP_HEADER {
private:
	char type[2];
	int32_t filesize;
	int32_t reserve;
	int32_t offset;
public:
	BITMAP_HEADER() {
		type[0] = 0;
		type[1] = 0;
		filesize = 0;
		reserve = 0;
		offset = 0;
	}
	void read(ifstream& is) {
		is.read((char*)(this), sizeof(BITMAP_HEADER));
	}
	string toString() {
		stringstream ss;
		ss << "class size: " << sizeof(BITMAP_HEADER) << endl;
		ss << "type: " << type[0] << type[1] << endl;
		ss << "file size: " << filesize << endl;
		ss << "offset: " << offset << endl;
		return ss.str();
	}
	const int FileSize() const {
		return filesize;
	}
	const int Offset() const {
		return offset;
	}
};
class DIB_HEADER {
private:
	int32_t header_size;
	int32_t width;
	int32_t height;
	int16_t plane;
	int16_t depth;
	int32_t compress;
	int32_t image_size;
	int32_t resolution_horizontal;
	int32_t resolution_vertical;
	int32_t palette;
	int32_t important_color;
public:
	DIB_HEADER() {
		header_size = 0;
		width = 0;
		height = 0;
		plane = 0;
		depth = 0;
		compress = 0;
		image_size = 0;
		resolution_horizontal = 0;
		resolution_vertical = 0;
		palette = 0;
		important_color = 0;
	}
	void read(ifstream& is) {
		is.read(reinterpret_cast<char*>(this), sizeof(DIB_HEADER));
	}
	string toString() {
		stringstream ss;
		ss << "header size: " << header_size << endl;
		ss << "width: " << width << endl;
		ss << "height:" << height << endl;
		ss << "plane: " << plane << endl;
		ss << "depth: " << depth << endl;
		ss << "compresion: " << compress << endl;
		ss << "image size: " << image_size << endl;
		ss << "horizontal resolution: " << resolution_horizontal << endl;
		ss << "vertical resolution: " << resolution_vertical << endl;
		ss << "palette: " << palette << endl;
		ss << "important color: " << important_color << endl;
		return ss.str();
	}
	int Width() {
		return width;
	}
	int Height() {
		return height;
	}
	int ImageSize() {
		return image_size;
	}
};
class Bitmap
{
private:
	BITMAP_HEADER bitmap_header;
	DIB_HEADER dib_header;
public:
	char* buf;
	Bitmap() { buf = 0; }
	~Bitmap() { if (buf) delete[] buf; }
	void Load(string filename)
	{
		ifstream file;
		file.open(filename, fstream::binary);
		bitmap_header.read(file);
		dib_header.read(file);
		int rawsize = dib_header.ImageSize();
		buf = new char[rawsize];
		file.read(buf, rawsize);
		file.close();
	}
	void Write(string filename)
	{
		ofstream outfile;
		outfile.open(filename, fstream::binary);
		outfile.write((char*)(&bitmap_header), sizeof(bitmap_header));
		outfile.write((char*)(&dib_header), sizeof(dib_header));
		int rawsize = dib_header.ImageSize();
		outfile.write(buf, rawsize);
		outfile.close();
	}
	string toString()
	{
		stringstream ss;
		ss << "[bitmap header]" << endl;
		ss << bitmap_header.toString();
		ss << "[dib header]" << endl;
		ss << dib_header.toString();
		return ss.str();
	}
	int Width()
	{
		return dib_header.Width();
	}
	int Height()
	{
		return dib_header.Height();
	}
	int imageSize()
	{
		return dib_header.ImageSize();
	}
	void Smooth()
	{
		double M[3][3] = { {1. / 9.,1. / 9., 1. / 9.} ,{1. / 9.,1. / 9., 1. / 9.} ,{1. / 9.,1. / 9., 1. / 9.} };
		doFilter(M);
	}
	void toGray()
	{
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int gray = 0;
		unsigned char R = 0, G = 0, B = 0;
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;
		int i;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				i = y * RowSize + (x * 3) % RowSize;
				B = buf[i];
				G = buf[i + 1];
				R = buf[i + 2];
				gray = int(0.299 * R + 0.587 * G + 0.114 * B);
				if (gray > 255) gray = 255;
				if (gray < 0) gray = 0;
				buf[i] = buf[i + 1] = buf[i + 2] = gray;
			}
		}
	}
	void toBiLevel(int threshold)
	{
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int gray = 0;
		unsigned char R = 0, G = 0, B = 0;
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;
		int i;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				i = y * RowSize + (x * 3) % RowSize;
				B = buf[i];
				G = buf[i + 1];
				R = buf[i + 2];
				gray = int(0.299 * R + 0.587 * G + 0.114 * B);
				if (gray > threshold) gray = 255;
				else gray = 0;
				buf[i] = buf[i + 1] = buf[i + 2] = gray;
			}
		}
	}
	void doFilter(double M[][3])
	{
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		unsigned char gray = 0;
		unsigned char R = 0, G = 0, B = 0;
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;
		int i;
		unsigned char** A;
		A = new unsigned char* [height];
		for (i = 0; i < height; i++)
			A[i] = new unsigned char[width];
		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
			{
				i = y * RowSize + (x * 3) % RowSize;
				A[y][x] = buf[i];
			}
		int m, n;
		for (int y = 1; y < height - 1; y++)
		{
			for (int x = 1; x < width - 1; x++)
			{
				i = y * RowSize + (x * 3) % RowSize;
				gray = 0;
				for (m = 0; m < 3; m++)
					for (n = 0; n < 3; n++)
						gray += A[y + m - 1][x + n - 1] * M[m][n];
				buf[i] = buf[i + 1] = buf[i + 2] = gray;
				if (gray > 255) gray = 255;
				if (gray < 0) gray = 0;
			}
		}
	}
	void toFusion(const char* otherBuf, Bitmap& out) {			//融合
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;
		int i;
		unsigned char B = 0, G = 0, R = 0;			//暫存 unsigned char buf 的BGR		(0~255)
		unsigned char B2 = 0, G2 = 0, R2 = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				i = y * RowSize + (x * 3) % RowSize;
				B = buf[i];		G = buf[i + 1];		R = buf[i + 2];						//第一張圖的BGR
				B2 = otherBuf[i];	G2 = otherBuf[i + 1];	R2 = otherBuf[i + 2];		//第二張圖的i~i+2位置的BGR
				if (B2 != 255 && G2 != 255 && R2 != 255) {
					out.buf[i] = 0.5 * B + 0.5 * B2;				//將兩張圖的BGR各乘0.5後相加
					out.buf[i + 1] = 0.5 * G + 0.5 * G2;
					out.buf[i + 2] = 0.5 * R + 0.5 * R2;
				}
			}
		}
	}
	void toDistortion(const int degree, Bitmap& out) {
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;
		int i, i2;
		int Ny = 0, Nx = 0;
		int midx, midy;		//中心
		midx = width / 2;
		midy = height / 2;
		double theta;		//角度
		double radius;		//半徑
		int offsetX;	//偏移量
		int offsetY;	//偏移量	
		for (int y = 0; y < height; y++) {
			offsetY = y - midy;			//此點與原點的偏移量
			for (int x = 0; x < width; x++) {
				offsetX = x - midx;		//此點與原點的偏移量

				theta = atan2(offsetY, offsetX);			//計算角度
				radius = sqrt(offsetX * offsetX + offsetY * offsetY);		//計算半徑

				radius = sqrt(radius) * degree;				//原座標半徑，產生扭曲效果

				Ny = radius * sin(theta) + midy;			//變回直角座標系
				Nx = radius * cos(theta) + midx;

				if (Ny < 0)	Ny = 0;				//超出邊界直接用邊界像素替代
				if (Ny >= height)	Ny = height - 1;
				if (Nx < 0)	Nx = 0;
				if (Nx >= width)	Nx = width - 1;

				i = y * RowSize + (x * 3) % RowSize;		//將像素寫進新out.buf中
				i2 = Ny * RowSize + (Nx * 3) % RowSize;
				out.buf[i2] = buf[i];
				out.buf[i2 + 1] = buf[i + 1];
				out.buf[i2 + 2] = buf[i + 2];

			}
		}
	}
	int Recognition(const int num[]) {			//回傳可能為此圖像的數字
		int i, j, k;
		int pixelRow = 0;
		for (j = 0; j < Height(); j++) {
			for (k = 0; k < Width(); k++) {
				int BitsPerPixel = 24;
				int RowSize = ((BitsPerPixel * Width() + 31) / 32) * 4;
				int unit = j * RowSize + (k * 3) % RowSize;
				unsigned char tmp = buf[unit];
				if (tmp == 0) {
					pixelRow++;
				}
			}
		}
		//計算pixelColumn
		int pixelColumn = 0;
		for (j = 0; j < Width(); j++) {
			for (k = 0; k < Height(); k++) {
				int BitsPerPixel = 24;
				int RowSize = ((BitsPerPixel * Width() + 31) / 32) * 4;
				int unit = k * RowSize + (j * 3) % RowSize;
				unsigned char tmp = buf[unit];
				if (tmp == 0) {
					pixelColumn++;
				}
			}
		}
		int pixelPerNum = pixelRow + pixelColumn;

		cout << "img.pixelPerNum" << pixelPerNum << endl;

		int tmpNum = 0;
		int tmpPixel = 10000000000;
		for (i = 0; i < 10; i++) {
			if (abs(pixelPerNum - num[i]) <= tmpPixel) {
				tmpPixel = abs(pixelPerNum - num[i]);
				tmpNum = i;
			}
		}
		return tmpNum;
	}
};
#pragma pack(pop)
void demo() {
	int i = 0, j = 0, k = 0;
	string str;
	string n;
	int pixelRow = 0;
	int pixelColumn = 0;
	int pixelPerNum[10];	//每一數字行與列的數量相加
	Bitmap num[10];		//存數字bmp


	//載入數字 0~9 圖片，並二值化 (0~9的檔名為N0~N9.bmp)
	for (i = 0; i < 10; i++) {
		str = "";
		n = to_string(i);
		str.append("N").append(n).append(".bmp");
		num[i].Load(str);
		num[i].toBiLevel(50);
	}
	
	for (i = 0; i < 10; i++) {			//0~9
		//計算 pixelRow
		pixelRow = 0;
		for (j = 0; j < num[i].Height(); j++) {
			for (k = 0; k < num[i].Width(); k++) {
				int BitsPerPixel = 24;
				int RowSize = ((BitsPerPixel * num[i].Width() + 31) / 32) * 4;
				int unit = j * RowSize + (k * 3) % RowSize;
				unsigned char tmp = num[i].buf[unit];
				if ((int)tmp == 0) {
					pixelRow++;
				}
			}
		}

		//計算pixelColumn
		pixelColumn = 0;
		for (j = 0; j < num[i].Width(); j++) {
			for (k = 0; k < num[i].Height(); k++) {
				int BitsPerPixel = 24;
				int RowSize = ((BitsPerPixel * num[i].Width() + 31) / 32) * 4;
				int unit = k * RowSize + (j * 3) % RowSize;
				unsigned char tmp = num[i].buf[unit];
				if ((int)tmp == 0) {
					pixelColumn++;
				}
			}
		}

		pixelPerNum[i] = pixelRow + pixelColumn;		//0~9 的row + column 的 pixel 數
	}
	
	for (i = 0; i < 10; i++) {
		cout << pixelPerNum[i] << " ";
	}
	
	Bitmap bmp1;			//將要進行識別的圖片
	bmp1.Load("img1.bmp");
	bmp1.toBiLevel(50);		//二值化

	int ans = bmp1.Recognition(pixelPerNum);
	cout << "ans:" << ans;
	
}
int main(int argc, char** argv) {
	cout << "[demo]" << endl;
	demo();
	return 0;
}