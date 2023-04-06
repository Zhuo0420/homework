#include <iostream>
using namespace std;

template <class T>				//資料型態參數化
T add(T x, T y) {				//加法函數
	return x + y;				//呼叫operator overloading
}
template <class T>
T sub(T x, T y) {				//減法函數
	return x - y;				//呼叫operator overloading
}
template <class T>
T mul(T x, T y) {				//乘法函數
	return x * y;				//呼叫operator overloading
}
template <class T>
T div(T x, T y) {				//除法函數
	return x / y;				//呼叫operator overloading
}
class VECTOR {
public:
	int x;
	int y;
	VECTOR(int x, int y) {			//建構子，傳入參數x,y
		set(x, y);					//呼叫set函數，並將x,y傳入
	}
	void set(int x, int y) {		//設定此點類別的data member x, y為參數x, y
		this->x = x;
		this->y = y;
	}
	VECTOR operator+(VECTOR other) {		//加法的operator overliading函數
		VECTOR tmp(x + other.x, y + other.y);
		return tmp;
	}
	VECTOR operator-(VECTOR other) {		//減法的operator overliading函數
		VECTOR tmp(x - other.x, y - other.y);
		return tmp;
	}
	VECTOR operator*(VECTOR other) {		//乘法的operator overliading函數
		VECTOR tmp(x * other.x, y * other.y);
		return tmp;
	}
	VECTOR operator/(VECTOR& other){		//除法的operator overliading函數
		VECTOR result(x / other.x, y / other.y);
		return result;
	}
	friend ostream& operator<<(ostream& os, const VECTOR other) {		//輸出的operator overliading函數
		os << "(" << other.x << "," << other.y << ")";
		return os;
	}
};

int main()
{
	VECTOR v1(3, 10);
	VECTOR v2(3, 4);
	cout << add(v1, v2) << endl;
	cout << sub(v1, v2) << endl;
	cout << mul(v1, v2) << endl;
	cout << div(v1, v2) << endl;
	return 0;
}