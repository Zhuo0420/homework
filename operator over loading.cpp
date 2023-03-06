#include <iostream>
using namespace std;
class MATRIX
{
private:
    int x;      //row
    int y;      //col
    double** arr;
public:
    MATRIX(int x, int y) {          //constructor
        this->x = x;
        this->y = y;
        arr = new double*[this->x];             //動態記憶體宣告x*y大小的陣列arr
        for (int i = 0; i < this->x; i++) {
            arr[i] = new double[this->y];
        }
    }
    friend ostream& operator<<(ostream& os, const MATRIX& v){        //輸出運算子
        for (int i = 0; i < v.x; i++) {
            for (int j = 0; j < v.y; j++) {
                os << v.arr[i][j] << " ";               //輸出v的arr中的值
            }
            os << endl;
        }
        return os;
    }
    friend istream& operator>>(istream& is, MATRIX& v)              //輸入運算子
    {
        for (int i = 0; i < v.x; i++) {
            for (int j = 0; j < v.y; j++) {
                is >> v.arr[i][j];                      //將輸入值給值於v的arr中
            }
        }
        return is;
    }
    MATRIX operator*(const double  n){                  //矩陣乘數值的乘法運算子
        MATRIX tmp(x, y);
        for (int i = 0; i < tmp.x; i++) {
            for (int j = 0; j < tmp.y; j++) {
                tmp.arr[i][j] *= n;                     //將矩陣中每一元素乘上變數n
            }
        }
        return tmp;                     //回傳乘過的矩陣
    }
    friend MATRIX operator*(double n, MATRIX v)         //數值乘矩陣的乘法運算子
    {
        for (int i = 0; i < v.x; i++) {
            for (int j = 0; j < v.y; j++) {
                v.arr[i][j] *= n;                       //將矩陣中每一元素乘上變數n
            }
        }
        return v;                       //回傳乘過的矩陣
    }
    friend MATRIX operator*(MATRIX a, MATRIX b) {       //矩陣乘矩陣的乘法運算子
        MATRIX tmp(a.x,b.y);
        for (int i = 0; i < tmp.x; i++) {
            for (int j = 0; j < tmp.y; j++) {
                tmp.arr[i][j] = 0;                      //將tmp的arr的初值設為0
            }
        }
        for (int i = 0; i < tmp.x; i++) {
            for (int j = 0; j < tmp.y; j++) {
                for (int k = 0; k < a.y; k++) {
                    tmp.arr[i][j] += a.arr[i][k] * b.arr[k][j];         //tmp[i][j]中的值為a矩陣的i列與b矩陣的j行互乘相加
                }
            }
        }
        return tmp;                 //回傳乘過的矩陣
    }
    MATRIX& operator=(const MATRIX& other)              //等於運算子
    {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                arr[i][j]=other.arr[i][j];              //等於的後項的元素給值於前項
            }
        }
        return *this;               //回傳此物件
    }
};
int main()
{
    MATRIX m1(3, 2), m2(2, 3), m(3, 3);
    cin >> m1 >> m2;
    m = m1 * m2;
    cout << m;
    m = 2 * m;
    cout << m << endl;
    return 0;
}
