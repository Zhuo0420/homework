#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

template<class T >
class POLYNOMIAL {					//可存多項式的class
private:
	vector<T> p;					//vector存放係數，順序為升冪
public:
	POLYNOMIAL() {

	}
	POLYNOMIAL(int num) {			
		p.resize(num + 1);			//vector大小為傳入值在加一
	}
	friend istream& operator>>(istream& is, const POLYNOMIAL<T>& me) {		//輸入運算子
		T tmp;
		for (int i = 0; i < me.p.size(); i++) {
			is >> tmp;
			me.p[i] = tmp;
		}
		return is;
	}
	friend ostream& operator<< (ostream& os, POLYNOMIAL<T> other) {			//輸出運算子
		int length = other.p.size();
		for (int i = length - 1; i >= 0; i--) {		//降冪輸出
			if (other.p[i]) {				//係數為零不輸出
				if (i == 0) {				//次方為0
					os << char(other.p[i] > 0 ? '+' : '-') << abs(other.p[i]);
				}
				else if (i == 1) {		//次方為1
					if (other.p[i] == 1) {		//係數為1
						os << char(other.p[i] > 0 ? '+' : '-') << "x";		//輸出 + or - x
					}
					else {
						os << char(other.p[i] > 0 ? '+' : '-') << abs(other.p[i]) << "x";		
					}
				}
				else if (i == length - 1) {			//最高次方，正數不輸出+
					if (other.p[i] == 1) {
						os << "x^" << i;
					}
					else {
						os <<other.p[i]<< "x^" << i;
					}
				}
				else {								
					if (other.p[i] == 1) {
						os << char(other.p[i] > 0 ? '+' : '-') << "x^" << i;
					}
					else {
						os << char(other.p[i] > 0 ? '+' : '-') << abs(other.p[i]) << "x^" << i;
					}
				}
			}
		}
		return os;
	}
	POLYNOMIAL operator=(T num){			//等於運算子，[] 執行完回傳位址後呼叫
		*this = num;						//num 給至此位址
		return *this;
	}
	POLYNOMIAL operator+(POLYNOMIAL<T>& other) {
		bool f = p.size() > other.p.size() ? 0 : 1;			//標記較大的vector長度
		int length;
		POLYNOMIAL<double> tmp;
		if (!f) { 
			length = other.p.size();			//此vector較大，使 tmp 內容與此物件一樣，length 設為較小的物件的大小
			tmp = *this;
		}
		else { 
			length = p.size(); 
			tmp = other;
		}
		for (int i = 0; i < length; i++) {			//將有較小長度的物件中的vector中的元素加到tmp中
			if (!f)	{
				tmp.p[i] = p[i];
				tmp.p[i] += other.p[i];				
			}
			else {
				tmp.p[i] = other.p[i]; 
				tmp.p[i] += p[i];
			}
		}
		return tmp;
	}
	POLYNOMIAL operator*(POLYNOMIAL<T>& other) {			//乘法運算子
		POLYNOMIAL<T> tmp(p.size() + other.p.size() - 2);		//宣告相乘後會有的最大次方數的物件
		for (int i = 0; i < p.size(); i++) {
			for (int j = 0; j < other.p.size(); j++) {
				tmp[i + j] += p[i] * other.p[j];				//tmp 中元素為對應次方係數乘機積
			}
		}
		return tmp;
	}
	T operator()(int num) {						//小括弧的運算子，參數為x的值，回傳此多項式值
		T sum = 0;
		for (int i = 0; i < p.size(); i++) {
			sum += (p[i] * pow(num, i));			
		}
		return sum;
	}
	T& operator[](int num) {				//中括號運算子，參數為第 num 次方數，回傳此物件 vector 的 num 位置的元素
		return p[num];
	}
};
int main()
{
	POLYNOMIAL<double> p1(5);
	POLYNOMIAL<double> p2(3);
	POLYNOMIAL<double> p3;
	POLYNOMIAL<double> p4;
	p1[0] = 1; p1[1] = 4; p1[2] = 5; p1[4] = -7; p1[5] = 2;
	p2[1] = 1; p2[3] = 2;
	p3 = p1 + p2;
	p4 = p1 * p2;
	cout << p1 << endl;
	cout << p2 << endl;
	cout << p3 << endl;
	cout << p4 << endl;
	cout << p1(2) << endl;
	return 0;
}
