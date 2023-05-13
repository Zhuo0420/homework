#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<cmath>
#define M_PI 3.1415926
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
		/*
		is.read(reinterpret_cast<char*>(&header_size),sizeof(int32_t));
		is.read(reinterpret_cast<char*>(&width),sizeof(int32_t));
		is.read(reinterpret_cast<char*>(&height),sizeof(int32_t));
		is.read(reinterpret_cast<char*>(&plane),sizeof(int16_t));
		is.read(reinterpret_cast<char*>(&depth),sizeof(int16_t));
		is.read(reinterpret_cast<char*>(&compress),sizeof(int32_t));
		is.read(reinterpret_cast<char*>(&image_size),sizeof(int32_t));
		is.read(reinterpret_cast<char*>(&resoltion_horizontal),sizeof(int32_t));
		is.read(reinterpret_cast<char*>(&resolution_vertical),sizeof(int32_t));
		is.read(reinterpret_cast<char*>(&palette),sizeof(int32_t));
		is.read(reinterpret_cast<char*>(&important_color),sizeof(int32_t));
		*/
		/*
		is>>header_size;
		is>>width;
		is>>height;
		is>>plane;
		is>>depth;
		is>>compress;
		is>>image_size;
		is>>resoltion_horizontal;
		is>>resolution_vertical;
		is>>palette;
		is>>important_color;
		*/
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
#pragma pack(pop)
void
demo() {
	ifstream file;
	file.open("img.bmp", fstream::binary);			//開檔
	BITMAP_HEADER bitmap_header;
	bitmap_header.read(file);						//呼叫 member function 讀檔
	DIB_HEADER dib_header;
	dib_header.read(file);							//呼叫 member function 讀檔
	int rawsize = dib_header.ImageSize();			
	char* buf;										//存放圖像內容
	buf = new char[rawsize];
	file.read(buf, rawsize);						//讀檔，將file中rawsize大小的內容存進buf裡
	file.close();
	cout << "[bitmap header]" << endl;
	cout << bitmap_header.toString();
	cout << "[dib header]" << endl;
	cout << dib_header.toString();
	ofstream outfile;
	outfile.open("img2.bmp", fstream::binary);		//開檔
	outfile.write((char*)(&bitmap_header), sizeof(bitmap_header));		//將 img1 的內容寫進 img2 中
	outfile.write((char*)(&dib_header), sizeof(dib_header));

	char** A;			//動態記憶體宣告
	A = new char * [30];
	for (int i = 0; i < 30; i++) {
		A[i] = new char[120];
	}

	for (int i = 0; i < rawsize - 3; i+=3) {					//將圖片轉為灰階
		int B = buf[i];
		int G = buf[i + 1];
		int R = buf[i + 2];
		buf[i] = buf[i + 1] = buf[i + 2] = char(0.299 * R + 0.587 * G + 0.114 * B);
	}

	int k = 0;			//計算buf中的位置編號
	for (int i = 0; i < 30; i++) {
		k = 3 * (dib_header.Height() / 30) * i * dib_header.Width();			//跳著讀取列 每圖片中 512/30 row 讀一次
		for (int j = 0; j < 120; j++) {
			A[i][j] = (int)buf[k];			//將buf內容存入A中
			k += 3*(dib_header.Width()/120);		//跳著讀取行 每 512/120 col 讀一次
		}
	}

	/*
	cout << "測試用 A中的內容 : " << endl;
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 120; j++) {
			cout <<(int)A[i][j]+127<< ' ';
		}
	}
	*/
	
	char T[5] = {' ', '.', '*', 'o', '@'};		//0~4，由淺到深

	outfile.write(buf, rawsize);				//寫檔，將rawsize大小的buf中的內容寫入
	outfile.close();
	delete[] buf;
	system("img.bmp");
	system("img2.bmp");

	ofstream Ofile;
	Ofile.open("Text.txt");					//開啟文字檔

	for (int i = 29; i >= 0; i--) {			//上下反轉
		for (int j = 0; j < 120; j++) {
			cout << T[(int)(A[i][j] + 127) / (256 / 5)];
			Ofile << T[(int)(A[i][j] + 127) / (256 / 5)];			//用查表法 buf中內容 /(256/5)來判斷要使用0~4哪個位置字元
		}
		cout << endl;
		Ofile << endl;
	}
	Ofile.close();

}
int main(int argc, char** argv) {
	cout << "[demo]" << endl;
	demo();
	return 0;
}
