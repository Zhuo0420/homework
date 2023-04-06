#include <iostream>
#include <string>
#include <exception>
#define DEBUG
using namespace std;

template<class T>
class COMPLEX {					//複數 data member 跟 member funcion 的 class
private:
	T re;
	T im;
public:
	COMPLEX() {					//無參數的建構子，將 data member 全設為零
		re = 0;
		im = 0;
	}
	COMPLEX(const T& re, const T& im) {		//有參數 re im 的建構子，將 data member 給值
		this->re = re;
		this->im = im;
	}
	COMPLEX complement() {				//共軛複數
		return COMPLEX(re, -im);
	}
	T magnitude()const {			//實部平方減虛部平方
		T tmp = re * re - im * im;
		if (tmp == 0)	throw exception("分母為零未定義");		//執回例外狀況，分母為零
		return tmp;
	}
	COMPLEX& operator=(const T& other) {		//參數為實數的等於運算子，將後者實數給值至此實數
		re = other;
		return *this;
	}
	COMPLEX& operator+=(const COMPLEX& other) {		//+=的運算子，此數等於此數加上後者的數
		re += other.re;
		im += other.im;
		return *this;
	}
	COMPLEX operator+(const COMPLEX& other) {		//後者為COMPLEX物件的加法運算子
		return COMPLEX(re + other.re, im + other.im);
	}
	COMPLEX operator+(const T& other) {				//後者為實數的加法運算子
		return COMPLEX(re + other, im);
	}
	COMPLEX operator-(const COMPLEX& other) {		//減法運算子
		return  COMPLEX(re - other.re, im - other.im);
	}
	COMPLEX operator*(const COMPLEX& other) {				//乘法運算子
		return COMPLEX(re * other.re - im * other.im, re * other.im + im * other.re);
	}
	COMPLEX operator/(const T& other) {				//後者為實數的除法運算子
		return COMPLEX(re / other, im / other); 
	}
	COMPLEX operator/(COMPLEX other) {			//後者為COMPLEX物件的除法運算子
		try {
			other.magnitude();				//當分母為零執回例外狀況
			return ((*this) * other.complement()) / other.magnitude();
		}
		catch(exception e){
			cout << e.what() << endl;		//輸出值回的例外狀況
			return *this;
		}
	}
	friend ostream& operator<<(ostream& os, const COMPLEX& other) {		//輸出運算子
		if (other.re)	os << other.re;			//物件中只有實數則只輸出實數
		if (other.re && other.im)	os << "+";		//物件中實數虛數皆有值，輸出兩者
		if (other.im)	os << other.im << "i";		//物件中只有虛數則只輸出虛數
		return os;
	}
	friend istream& operator>>(istream& is, COMPLEX& other) {		//輸入運算子
		is >> other.re >> other.im;
		return is;
	}
};

#ifdef DEBUG
void testCase_COMPLEX_1() {				//COMPLEX建構測資
	cout << "[test complex]" << endl;
	COMPLEX<int> c(1, 2);				
	cerr << c << endl;
}
void testCase_COMPLEX_2() {				//COMPLEX輸入測資
	cout << "[test complex input]" << endl;
	COMPLEX<int> c;
	cin >> c;
	cerr << c << endl;
}
void testCase_COMPLEX_3() {				//COMPLEX加法測資
	cout << "[test complex add]" << endl;
	COMPLEX<int>c1(1, 2), c2(3, 4), c;
	c = c1 + c2;
	cerr << c1<<"+" << c2 << "=" << c << endl;
}
void testCase_COMPLEX_4() {				//COMPLEX減法測資
	cout << "[test complex sub]" << endl;
	COMPLEX<int>c1(1, 2), c2(3, 4), c;
	c = c1 - c2;
	cerr << c1 << "-" << c2 << "=" << c << endl;
}
void testCase_COMPLEX_5() {				//COMPLEX乘法測資
	cout << "[test complex mul]" << endl;
	COMPLEX<int>c1(1, 2), c2(3, 4), c;
	c = c1 * c2;
	cerr << c1 << "*" << c2 << "=" << c << endl;
}
void testCase_COMPLEX_6() {				//COMPLEX除法測資
	cout << "[test complex div]" << endl;
	COMPLEX<int>c1(1, 2), c2(3, 4), c;
	c = c1 / c2;
	cerr <<"(" << c1 <<")" << "/" <<"(" << c2 <<")" << "=" << c << endl;
}
#endif // DEBUG

template<class T>
class MATRIX{				//矩陣  data member 跟 member funcion 的 class
private:
	int Col;
	int Row;
	T** A;
public:
	MATRIX(){				//無參數的建構子
		Col = 0;
		Row = 0;
		A = 0;
	}
	
	MATRIX(int row, int col){			//有行列值的建構子
		ReSize(row, col);
	}
	~MATRIX() {							//解構子
		Clear();
	}
	template<class U>					//copy constructor 但因編譯器 return value optimization 沒有被執行到
	MATRIX(const MATRIX<U>& other){
		cout << "Copy constructor called" << endl;
		ReSize(other.Col, other.Row);
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				A[i][j] = other.A[i][j];
			}
		}
	}
	template<class U>				//move comstruct (沒有被執行到)
	MATRIX(MATRIX<U>&& other) {
		cout<< "move constructor called" << endl;
		ReSize(other.Col, other.Row);
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				A[i][j] = other.A[i][j];
			}
		}
	}
	template<class U>
	MATRIX& operator=(MATRIX<U>&& other) {				//move assignment 將等號右側的運算式回傳的暫存物件直接移到左值中
		cout << "move assignment called" << endl;
		ReSize(other.Row, other.Col);
		int i, j;
		for (i = 0; i < Row; i++) {
			for (j = 0; j < Col; j++) {
				A[i][j] = other(i, j);
			}
		}
		return *this;
	}

 	void ReSize(int row, int col){					//動態宣告A矩陣
		Col = col;
		Row = row;
		if (A) Clear();						//如A不為空，清掉
		int i;
		A = new T * [Row];
		for (i = 0; i < Row; i++)
			A[i] = new T[Col];
	}
	void Clear(){						//釋放A中記憶體
		int i;
		for (i = 0; i < Row; i++)
			delete[] A[i];
		delete[] A;
	}
	void zero(){			//將A中元素給值	0
		int i;
		int j;
		for (i = 0; i < Row; i++)
			for (j = 0; j < Col; j++) {
				A[i][j] = 0;
			}
	}
	template<class U>
	MATRIX& operator=(const MATRIX<U> other) const{				//等於運算子
		ReSize(other.Row, other.Col);
		int i, j;
		for (i = 0; i < Row; i++){
			for (j = 0; j < Col; j++){
				A[i][j] = other(i, j);
			}
		}
	}
	MATRIX operator+(const MATRIX& other) const	{				//加法運算子
		if (Row == other.Row && Col == other.Col){				//如前後項行列不同時執回例外狀況
			int i;
			int j;
			MATRIX<T>answer(Row, Col);
			for (i = 0; i < Row; i++){
				for (j = 0; j < Col; j++){
					answer(i, j) = A[i][j] + other(i, j);
				}
			}
			return answer;					//回傳暫存的
		}
		else{
			throw exception("size of tow matrix different");
		}
	}

	MATRIX operator-(const MATRIX& other) const {					//減法運算子
		if (Row == other.Row && Col == other.Col){					//如前後項行列不同時執回例外狀況
			int i;
			int j;
			MATRIX<T>answer(Row, Col);
			for (i = 0; i < Row; i++)
			{
				for (j = 0; j < Col; j++)
				{
					answer(i, j) = A[i][j] - other(i, j);
				}
			}
			return answer;
		}
		else{
			throw exception("size of tow matrix different");
		}
	}
	MATRIX operator*(const MATRIX& other) const	{					//乘法運算子
		if (Col == other.Row){										//如前項 col 不等於 後項 row，不可乘，執回例外狀況
			int i;
			int j;
			int k;
			MATRIX<T>answer(Row, other.Col);
			for (i = 0; i < answer.Col; i++){
				for (j = 0; j < answer.Col; j++){
					for (k = 0; k < Col; k++)
					{
						answer(i, j) += A[i][k] * other(k, j);
					}
				}
			}
			return answer;				//呼叫 move constructor (但沒有成功呼叫)
		}
		else{
			throw exception("size of two matrix is wrong");
		}
	}

	T& operator()(int r, int c) const{					//小括弧的運算子
		return A[r][c];					//回傳A中r列c行元素
	}
	friend ostream& operator<<(ostream& os, MATRIX& m){				//輸出運算子
		int i;
		int j;
		for (i = 0; i < m.Row; i++){			//輸出m的A中的元素
			for (j = 0; j < m.Col; j++)
				os << m(i, j) << "\t";
			os << endl;
		}
		return os;
	}
	friend istream& operator >> (istream& is, MATRIX& m){				//輸入運算子
		int i;
		int j;
		for (i = 0; i < m.Row; i++){					//輸入值至m的A中
			for (j = 0; j < m.Col; j++)
				is >> m(i, j);
		}
		return is;
	}
};

#ifdef DEBUG
void testCase_MATRIX_1() {
	cout << "[test declare and output]" << endl;				//宣告跟輸出MATRIX的測資
	MATRIX<COMPLEX<double>> m(2, 3);
	cout << m << endl;
}
void testCase_MATRIX_2() {					//輸入MATRIX的測資
	cout << "[test declare and input]" << endl;
	MATRIX<COMPLEX<double>> m(2, 3);
	cin >> m;
	cout << m << endl;
}
void testCase_MATRIX_3() {					//MATRIX加法的測資
	cout << "[test add tow matrix]" << endl;
	MATRIX<COMPLEX<double>> m1(2, 3);
	MATRIX<COMPLEX<double>> m2(2, 3);
	MATRIX<COMPLEX<double>> m3;
	m1(0, 0) = COMPLEX<double>(1, 2);
	m1(0, 1) = COMPLEX<double>(3, 2);
	m1(0, 2) = 3;
	m1(1, 0) = 4;
	m1(1, 1) = 5;
	m1(1, 2) = 6;
	m2(0, 0) = 1;
	m2(0, 1) = 1;
	m2(0, 2) = 1;
	m2(1, 0) = 1;
	m2(1, 1) = 1;
	m2(1, 2) = 1;
	try {
		m3 = m1 + m2;					//會呼叫 move assignment
		cout << m1 << endl << m2 << endl << m3 << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;					//輸出被執回的例外狀況
	}
}
void testCase_MATRIX_4() {					//MATRIX加法的測資(會因矩陣大小不同執回例外狀況)
	cout << "[test add tow matrix]" << endl;
	MATRIX<COMPLEX<double>> m1(2, 3);
	MATRIX<COMPLEX<double>> m2(5, 3);
	MATRIX<COMPLEX<double>> m3;
	try {
		m3 = m1 + m2;					//會呼叫 move assignment
		cout << m1 << endl << m2 << endl << m3 << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;			//輸出被執回的例外狀況
	}
}
void testCase_MATRIX_5() {					//MATRIX減法的測資
	cout << "[test sub tow matrix]" << endl;					
	MATRIX<COMPLEX<double>> m1(2, 3);
	MATRIX<COMPLEX<double>> m2(2, 3);
	MATRIX<COMPLEX<double>> m3;
	m1(0, 0) = COMPLEX<double>(1, 2);
	m1(0, 1) = COMPLEX<double>(3, 2);
	m1(0, 2) = 3;
	m1(1, 0) = 4;
	m1(1, 1) = 5;
	m1(1, 2) = 6;
	m2(0, 0) = 1;
	m2(0, 1) = 1;
	m2(0, 2) = 1;
	m2(1, 0) = 1;
	m2(1, 1) = 1;
	m2(1, 2) = 1;
	try {
		m3 = m1 - m2;						//會呼叫 move assignment
		cout << m1 << endl << m2 << endl << m3 << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;			//輸出被執回的例外狀況
	}
}
void testCase_MATRIX_6() {					//MATRIX乘法的測資
	cout << "[test mul tow matrix]" << endl;
	MATRIX<COMPLEX<double>> m1(2, 3);
	MATRIX<COMPLEX<double>> m2(3, 2);
	MATRIX<COMPLEX<double>> m3;
	m1(0, 0) = 1;
	m1(0, 1) = 2;
	m1(0, 2) = 3;
	m1(1, 0) = 4;
	m1(1, 1) = 5;
	m1(1, 2) = 6;
	m2(0, 0) = 1;
	m2(0, 1) = 1;
	m2(1, 0) = 1;
	m2(1, 1) = 1;
	m2(2, 0) = 1;
	m2(2, 1) = 1;
	try {
		m3 = m1 * m2;						//會呼叫 move assignment
		cout << m1 << endl << m2 << endl << m3 << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;				//輸出被執回的例外狀況
	}
}
void testCase_MATRIX_7(){
	cout << "[test mul two matrix]" << endl;		//MATRIX乘法的測資(會因矩陣大小不可乘而執回例外狀況)
	MATRIX<COMPLEX<double>> m1(2, 3);
	MATRIX<COMPLEX<double>> m2(2, 3);
	MATRIX<COMPLEX<double>> m3;
	m1(0, 0) = COMPLEX<double>(1, 2);
	m1(0, 1) = COMPLEX<double>(3, 2);
	m1(0, 2) = 3;
	m1(1, 0) = 4;
	m1(1, 2) = 6;
	m2(0, 0) = 1;
	m2(0, 1) = 1;
	m2(0, 2) = 1;
	m2(1, 0) = 1;
	m2(1, 1) = 1;
	m2(1, 2) = 1;
	try{
		m3 = m1 * m2;					//會呼叫 move assignment
		cout << m1 << endl << m2 << endl << m3 << endl;
	}
	catch (exception e){
		cout << e.what() << endl;				//輸出被執回的例外狀況
	}
}
#endif // !DEBUG


int main()
{
#ifdef DEBUG
	testCase_COMPLEX_1();
	testCase_COMPLEX_2();
	testCase_COMPLEX_3();
	testCase_COMPLEX_4();
	testCase_COMPLEX_5();
	testCase_COMPLEX_6();

	testCase_MATRIX_1();
	testCase_MATRIX_2();
	testCase_MATRIX_3();
	testCase_MATRIX_4();
	testCase_MATRIX_5();
	testCase_MATRIX_6();
	testCase_MATRIX_7();
#endif // DEBUG

}
