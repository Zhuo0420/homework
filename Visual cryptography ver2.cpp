#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <time.h>
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
class Bitmap {
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
	void toCMY(Bitmap arr[2][3]) {			//將圖像分解為CMY
		int x, y;
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;

		int otherWidth = arr[0][0].Width();
		int otherHeight = arr[0][0].Height();
		int otherRowsize = ((BitsPerPixel * otherWidth + 31) / 32) * 4;

		unsigned char R, G, B;

		int set1, set2;
		set1 = rand() % 2;
		set2 = (set1 == 1) ? 0 : 1;



		//改為兩對隨機位置

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {

				int posi[2][2];
				int num1 = rand() % 4;				//num1, num2 值為0~3，兩者不重複
				int num2 = rand() % 4;
				int i1 = y * RowSize + (x * 3) % RowSize;
				int i2 = y * 2 * otherRowsize + (x * 2 * 3) % otherRowsize;

				while (num1 == num2) {
					num2 = rand() % 4;
					//cout << num1 << num2 << endl;
				}

				//2*2方陣，值為1的位置為填入原顏色值的位置，方陣中值為0的位置表填入0的位置 
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 2; j++) {
						if (2 * i + j == num1 || 2 * i + j == num2) {
							posi[i][j] = 1;
						}
						else {
							posi[i][j] = 0;
						}
					}
				}

				R = buf[i1 + 2]; G = buf[i1 + 1]; B = buf[i1];
				int tmp1, tmp2;

				for (int row = 0; row < 2; row++) {
					for (int col = 0; col < 2; col++) {
						if (posi[row][col] == 1) {
							tmp1 = i2 + otherRowsize * row + (3 * col);
							arr[0][0].buf[tmp1 + 2] = R;
						}
						else {				//posi[row][col] == 0
							tmp2 = i2 + otherRowsize * row + (3 * col);
							arr[1][0].buf[tmp2 + 2] = 0;
						}
					}
				}

				/*
				tmp1 = i2 + otherRowsize * posi[1][0] + (3 * posi[0][0]);
				tmp2 = tmp1 +  (3 * posi[0][1]);
				arr[0][0].buf[tmp1 + 2] = R;
				arr[0][0].buf[tmp2 + 2] = R;

				arr[1][0].buf[tmp1 + 2] = 0;
				arr[1][0].buf[tmp2 + 2] = 0;
				*/

				num1 = rand() % 4;
				num2 = rand() % 4;
				while (num1 == num2) {
					num2 = rand() % 4;
					//cout << num1 << num2 << endl;
				}
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 2; j++) {
						if (2 * i + j == num1 || 2 * i + j == num2) {
							posi[i][j] = 1;
						}
						else {
							posi[i][j] = 0;
						}
					}
				}

				for (int row = 0; row < 2; row++) {
					for (int col = 0; col < 2; col++) {
						if (posi[row][col] == 1) {
							tmp1 = i2 + otherRowsize * row + (3 * col);
							arr[0][1].buf[tmp1 + 1] = G;
						}
						else {				//posi[row][col] == 0
							tmp2 = i2 + otherRowsize * row + (3 * col);
							arr[1][1].buf[tmp2 + 1] = 0;
						}
					}
				}

				/*
				tmp1 = i2 + otherRowsize * posi[1][0] + (3 * posi[0][0]);
				tmp2 = tmp1 + (3 * posi[0][1]);
				arr[0][1].buf[tmp1 + 1] = G;
				arr[0][1].buf[tmp2 + 1] = G;
				arr[1][1].buf[tmp1 + 1] = 0;
				arr[1][1].buf[tmp2 + 1] = 0;
				*/
				num1 = rand() % 4;
				num2 = rand() % 4;
				while (num1 == num2) {
					num2 = rand() % 4;
					//cout << num1 << num2 << endl;
				}
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 2; j++) {
						if (2 * i + j == num1 || 2 * i + j == num2) {
							posi[i][j] = 1;
						}
						else {
							posi[i][j] = 0;
						}
					}
				}

				for (int row = 0; row < 2; row++) {
					for (int col = 0; col < 2; col++) {
						if (posi[row][col] == 1) {
							tmp1 = i2 + otherRowsize * row + (3 * col);
							arr[0][2].buf[tmp1] = B;
						}
						else {				//posi[row][col] == 0
							tmp2 = i2 + otherRowsize * row + (3 * col);
							arr[1][2].buf[tmp2] = 0;
						}
					}
				}

				/*
				tmp1 = i2 + otherRowsize * posi[1][0] + (3 * posi[0][0]);
				tmp2 = tmp1 + (3 * posi[0][1]);
				arr[0][2].buf[tmp1 + 1] = B;
				arr[0][2].buf[tmp2 + 1] = B;
				arr[1][2].buf[tmp1 + 1] = 0;
				arr[1][2].buf[tmp2 + 1] = 0;
				*/
			}
		}
	}
	void toCombinationCMY(const Bitmap& Bm, const Bitmap& Gm, const Bitmap& Rm) {
		int x, y;
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				int i = y * RowSize + (x * 3) % RowSize;

				buf[i + 2] = Rm.buf[i + 2];
				buf[i + 1] = Gm.buf[i + 1];
				buf[i] = Bm.buf[i];
			}
		}
	};
	void toOverlay(const Bitmap& bmp1, const Bitmap& bmp2) {
		int x, y;
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;

		unsigned char R1, G1, B1;
		unsigned char R2, G2, B2;

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				int i = y * RowSize + (3 * x) % RowSize;

				B1 = bmp1.buf[i]; G1 = bmp1.buf[i + 1]; R1 = bmp1.buf[i + 2];
				B2 = bmp2.buf[i]; G2 = bmp2.buf[i + 1]; R2 = bmp2.buf[i + 2];


				//暴力重疊
				buf[i] = B1 * 0.5 + B2 * 0.5;
				buf[i + 1] = G1 * 0.5 + G2 * 0.5;
				buf[i + 2] = R1 * 0.5 + R2 * 0.5;


				/*
				//B的部分
				if (B1 != 255 && B2 != 255) {
					buf[i] = B1 * 0.5 + B2 * 0.5;
				}
				else if(B1 != 255) {
					buf[i] = B1;
				}
				else if(B2 != 255){
					buf[i] = B2;
				}
				else {
					buf[i] = 0;
				}

				//G的部分
				if (G1 != 255 && G2 != 255) {
					buf[i + 1] = G1 * 0.5 + G2 * 0.5;
				}
				else if (G1 != 255) {
					buf[i + 1] = G1;
				}
				else if (G2 != 255) {
					buf[i + 1] = G2;
				}
				else {
					buf[i + 1] = 0;
				}

				//R的部分
				if (R1 != 255 && R2 != 255) {
					buf[i + 2] = R1 * 0.5 + R2 * 0.5;
				}
				else if (R1 != 255) {
					buf[i + 2] = R1;
				}
				else if (R2 != 255) {
					buf[i + 2] = R2;
				}
				else {
					buf[i + 2] = 0;
				}
				*/
			}
		}
	}

	void toSeparado(Bitmap arr[3]) {			//將圖分為三個顏色分量
		int x, y;
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				int i = y * RowSize + (x * 3) % RowSize;

				//B
				arr[0].buf[i] = buf[i];
				arr[0].buf[i + 1] = 255;
				arr[0].buf[i + 2] = 255;

				//G
				arr[1].buf[i] = 255;
				arr[1].buf[i + 1] = this->buf[i + 1];
				arr[1].buf[i + 2] = 255;

				//R
				arr[2].buf[i] = 255;
				arr[2].buf[i + 1] = 255;
				arr[2].buf[i + 2] = this->buf[i + 2];
			}
		}
	}
	void halftone(int mid, int ca) {		//半色調, ca 1~3表 BGR 整張圖的錯誤擴散
		int x, y;
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;

		unsigned char R, G, B;

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				int i = y * RowSize + (3 * x) % RowSize;
				B = buf[i]; G = buf[i + 1]; R = buf[i + 2];			//此點原本的數值

				//誤差值
				int div;
				//暫存運算數值
				int tmp;

				switch (ca) {
				case 1:
					buf[i] = (B >= mid) ? 255 : 0;
					div = B - buf[i];
					//cout << div << " ";
					if (x + 1 < width) {
						int j = y * RowSize + ((x + 1) * 3) % RowSize;
						tmp = div * 7 / 16;
						if (buf[j] + tmp > 255) {
							buf[j] = 255;
						}
						else if (buf[j] + tmp < 0) {
							buf[j] = 0;
						}
						else {
							buf[j] += tmp;
						}
					}
					if (x - 1 > 0 && y + 1 < height) {
						int j = (y + 1) * RowSize + ((x - 1) * 3) % RowSize;
						tmp = div * 3 / 16;
						if (buf[j] + tmp > 255) {
							buf[j] = 255;
						}
						else if (buf[j] + tmp < 0) {
							buf[j] = 0;
						}
						else {
							buf[j] += tmp;
						}
					}
					if (y + 1 < height) {
						int j = (y + 1) * RowSize + ((x) * 3) % RowSize;
						tmp = div * 5 / 16;
						if (buf[j] + tmp > 255) {
							buf[j] = 255;
						}
						else if (buf[j] + tmp < 0) {
							buf[j] = 0;
						}
						else {
							buf[j] += tmp;
						}
					}
					if (x + 1 < width && y + 1 < height) {
						int j = (y + 1) * RowSize + ((x + 1) * 3) % RowSize;
						tmp = div * 1 / 16;
						if (buf[j] + tmp > 255) {
							buf[j] = 255;
						}
						else if (buf[j] + tmp < 0) {
							buf[j] = 0;
						}
						else {
							buf[j] += tmp;
						}
					}
					break;
				case 2:
					buf[i + 1] = (G >= mid) ? 255 : 0;
					div = G - buf[i + 1];

					if (x + 1 < width) {
						int j = y * RowSize + ((x + 1) * 3) % RowSize;
						tmp = div * 7 / 16;
						if (buf[j + 1] + tmp > 255) {
							buf[j + 1] = 255;
						}
						else if (buf[j + 1] + tmp < 0) {
							buf[j + 1] = 0;
						}
						else {
							buf[j + 1] += tmp;
						}
					}
					if (x - 1 > 0 && y + 1 < height) {
						int j = (y + 1) * RowSize + ((x - 1) * 3) % RowSize;
						tmp = div * 3 / 16;
						if (buf[j + 1] + tmp > 255) {
							buf[j + 1] = 255;
						}
						else if (buf[j + 1] + tmp < 0) {
							buf[j + 1] = 0;
						}
						else {
							buf[j + 1] += tmp;
						}
					}
					if (y + 1 < height) {
						int j = (y + 1) * RowSize + ((x) * 3) % RowSize;
						tmp = div * 5 / 16;
						if (buf[j + 1] + tmp > 255) {
							buf[j + 1] = 255;
						}
						else if (buf[j + 1] + tmp < 0) {
							buf[j + 1] = 0;
						}
						else {
							buf[j + 1] += tmp;
						}
					}
					if (x + 1 < width && y + 1 < height) {
						int j = (y + 1) * RowSize + ((x + 1) * 3) % RowSize;
						tmp = div * 1 / 16;
						if (buf[j + 1] + tmp > 255) {
							buf[j + 1] = 255;
						}
						else if (buf[j + 1] + tmp < 0) {
							buf[j + 1] = 0;
						}
						else {
							buf[j + 1] += tmp;
						}
					}
					break;
				case 3:
					buf[i + 2] = (R >= mid) ? 255 : 0;
					div = R - buf[i + 2];

					if (x + 1 < width) {
						int j = y * RowSize + ((x + 1) * 3) % RowSize;
						tmp = div * 7 / 16;
						if (buf[j + 2] + tmp > 255) {
							buf[j + 2] = 255;
						}
						else if (buf[j + 2] + tmp < 0) {
							buf[j + 2] = 0;
						}
						else {
							buf[j + 2] += tmp;
						}
					}
					if (x - 1 > 0 && y + 1 < height) {
						int j = (y + 1) * RowSize + ((x - 1) * 3) % RowSize;
						tmp = div * 3 / 16;
						if (buf[j + 2] + tmp > 255) {
							buf[j + 2] = 255;
						}
						else if (buf[j + 2] + tmp < 0) {
							buf[j + 2] = 0;
						}
						else {
							buf[j + 2] += tmp;
						}
					}
					if (y + 1 < height) {
						int j = (y + 1) * RowSize + ((x) * 3) % RowSize;
						tmp = div * 5 / 16;
						if (buf[j + 2] + tmp > 255) {
							buf[j + 2] = 255;
						}
						else if (buf[j + 2] + tmp < 0) {
							buf[j + 2] = 0;
						}
						else {
							buf[j + 2] += tmp;
						}
					}
					if (x + 1 < width && y + 1 < height) {
						int j = (y + 1) * RowSize + ((x + 1) * 3) % RowSize;
						tmp = div * 1 / 16;
						if (buf[j + 2] + tmp > 255) {
							buf[j + 2] = 255;
						}
						else if (buf[j + 2] + tmp < 0) {
							buf[j + 2] = 0;
						}
						else {
							buf[j + 2] += tmp;
						}
					}
					break;
				}
			}
		}
	}
	void unitPixelHalftone(int y, int x, int div, int T, int ca) {
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;

		int i = y * RowSize + (x * 3) % RowSize;
		int j;

		if (x + 1 < width) {
			j = y * RowSize + ((x + 1) * 3) % RowSize;
			buf[j + ca] += div * 7 / 16;
		}
		if (x - 1 >= 0 && y - 1 >= 0) {
			j = (y - 1)*RowSize + ((x - 1) * 3) % RowSize;
			buf[j + ca] += div * 3 / 16;
		}
		if (y - 1 >= 0) {
			j = (y - 1)*RowSize + (x * 3) % RowSize;
			buf[j + ca] += div * 5 / 16;
		}
		if (x + 1 < width && y - 1 >= 0) {
			j = (y - 1)*RowSize + ((x + 1) * 3) % RowSize;
			buf[j + ca] += div / 16;
		}
		

	}
	void toEncryption(Bitmap &c1, Bitmap &c2, int T) {
		int x, y;
		int rawsize = imageSize();
		int width = Width();
		int height = Height();
		int BitsPerPixel = 24;
		int RowSize = ((BitsPerPixel * width + 31) / 32) * 4;

		unsigned char R1, G1, B1;
		unsigned char R2, G2, B2;
		int i;

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				int tmp1, tmp2;
				i = y * RowSize + (x * 3) % RowSize;
				B1 = c1.buf[i];			B2 = c2.buf[i];
				G1 = c1.buf[i + 1];		G2 = c2.buf[i + 1];
				R1 = c1.buf[i + 2];		R2 = c2.buf[i + 2];
				for (int k = 0; k < 3; k++) {
					if (buf[i + k] == 255) {
						c1.buf[i + k] = 255;
						c2.buf[i + k] = 255;
					}
					else if (buf[i + k] == 0) {
						if (c1.buf[i + k] >= T && c2.buf[i + k] >= T) {
							c1.buf[i + k] = (c1.buf[i + k] >= c2.buf[i]) ? 255 : 0;
							c2.buf[i + k] = (c2.buf[i + k] > c1.buf[i + k]) ? 255 : 0;
						}
						else if (c1.buf[i + k] >= T && c2.buf[i + k] < T) {
							c1.buf[i + k] = 255;	c2.buf[i + k] = 0;
						}
						else if (c1.buf[i + k] < T && c2.buf[i + k] >= T) {
							c1.buf[i + k] = 0;	c2.buf[i + k] = 255;
						}
						else {
							c1.buf[i + k] = 0;	c2.buf[i + k] = 0;
						}
					}

					switch (k){
					case 0:
						tmp1 = B1 - c1.buf[i];
						tmp2 = B2 - c2.buf[i];
						break;
					case 1:
						tmp1 = G1 - c1.buf[i+1];
						tmp2 = G2 - c2.buf[i+1];
						break;
					case 2:
						tmp1 = R1 - c1.buf[i+2];
						tmp2 = R2 - c2.buf[i+2];
						break;
					}
					
					c1.unitPixelHalftone(y, x, tmp1, T, k);
					c2.unitPixelHalftone(y, x, tmp2, T, k);
				}
			}
		}

	}

};

void demo1() {
	//宣告及載入圖片
	Bitmap bmp1;		//機密影像
	Bitmap bmp2;		//偽裝影像
	Bitmap bmp3;		//偽裝影像
	Bitmap overlayBmp;		//疊合影像

	bmp1.Load("img1.bmp");
	bmp2.Load("img2.bmp");
	bmp3.Load("img2.bmp");
	overlayBmp.Load("img2.bmp");

	int T = 180;

	bmp1.halftone(T, 1);
	bmp1.halftone(T, 2);
	bmp1.halftone(T, 3);

	bmp1.toEncryption(bmp2, bmp3, 128);
	
	overlayBmp.toOverlay(bmp2, bmp3);

	//半色調的機密影像
	bmp1.Write("sHalfTone.bmp");
	system("sHalfTone.bmp");

	bmp2.Write("output1.bmp");
	bmp3.Write("output2.bmp");
	overlayBmp.Write("overlay.bmp");
	
	
	system("output1.bmp");
	system("output2.bmp");
	system("overlay.bmp");
}


int main()
{
	demo1();
}
