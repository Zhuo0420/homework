#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;
#pragma pack(push)
#pragma pack(1)
class BITMAP_HEADER
{
private:
	char type[2];
	int32_t filesize;
	int32_t reserve;
	int32_t offset;
public:
	BITMAP_HEADER()
	{
	}
	void read(ifstream& is)
	{
		is.read((char*)(this), sizeof(BITMAP_HEADER));
	}
	string toString()
	{
		stringstream ss;
		ss << "class size: " << sizeof(BITMAP_HEADER) << endl;
		ss << "type: " << type[0] << type[1] << endl;
		ss << "file size: " << filesize << endl;
		ss << "offset: " << offset << endl;
		return ss.str();
	}
	const int FileSize() const { return filesize; }
	const int Offset() const { return offset; }
};
class DIB_HEADER
{
private:
	int32_t header_size;
	int32_t width;
	int32_t height;
	int16_t plane;
	int16_t depth;
	int32_t compress;
	int32_t image_size;
	int32_t resoltion_horizontal;
	int32_t resolution_vertical;
	int32_t palette;
	int32_t important_color;
public:
	DIB_HEADER()
	{
	}
	void read(ifstream& is)
	{
		is.read((char*)(this), sizeof(DIB_HEADER));
	}
	string toString()
	{
		stringstream ss;
		ss << "header size: " << header_size << endl;
		ss << "width: " << width << endl;
		ss << "height:" << height << endl;
		ss << "plane: " << plane << endl;
		ss << "dept: " << depth << endl;
		ss << "compress: " << compress << endl;
		ss << "image size: " << image_size << endl;
		ss << "horizontal resolution: " << resoltion_horizontal << endl;
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
};
#pragma pack(pop)
void
demo()
{
	ifstream file;
	file.open("img.bmp", fstream::binary);
	BITMAP_HEADER bitmap_header;
	bitmap_header.read(file);
	DIB_HEADER dib_header;
	dib_header.read(file);
	int rawsize = bitmap_header.FileSize() - bitmap_header.Offset();
	char* buf;
	buf = new char[rawsize];
	file.read(buf, rawsize);
	file.close();
	cout << bitmap_header.toString();
	cout << dib_header.toString();
	ofstream outfile;
	outfile.open("img2.bmp", fstream::binary);
	outfile.write((char*)(&bitmap_header), sizeof(bitmap_header));
	outfile.write((char*)(&dib_header), sizeof(dib_header));

	int x0 = dib_header.Width() / 2 / 2 + 15;		//圖中臉的位置 x
	int y0 = dib_header.Height() / 2 + 60;			//圖中臉的位置 y
	int k = y0 * dib_header.Width() * 3 + x0 * 3;	//一維陣列的位置

	int x;
	int y;
	int r = 50;		//半徑



	for (int i = 0; i < rawsize; i += 3)		//rawsize = bit*3
	{
		x = (i / 3) % dib_header.Width();		//現在位置 x
		y = i / dib_header.Width() / 3;			//現在位置 y
		float tmp = pow((x - x0), 2) + pow((y - y0), 2);
		if (tmp >= pow(100, 2))				//如果現在的點在圓內
		{
			tmp = sqrt(tmp);
			int B = buf[i + 2];		//藍
			int G = buf[i + 1];		//綠
			int R = buf[i];			//紅
			int gray = (R * 0.299 + G * 0.587 + B * 0.114);				//圓外的部分為灰階
			buf[i] = buf[i + 1] = buf[i + 2] = gray + (r - tmp);					//設定rgb為灰階
		}
	}

	outfile.write(buf, rawsize);
	outfile.close();
	delete[] buf;
	//system("img.bmp");
	system("img2.bmp");
}
int
main(int argc, char** argv)
{
	cout << "[demo]" << endl;
	demo();
	return 0;
}
