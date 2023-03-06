#include <iostream>
#include <algorithm>
using namespace std;

class MATRIX {
private:
    int** A;
    int col;
    int row;
    void clear()            //釋放動態記憶體
    {
        for (int i = 0; i < row; i++) {
            delete [] A[i];
        }
        delete[]A;

    }
    void init(int row, int col)         //動態記憶體宣告陣列A
    {
        this->col = col;
        this->row = row;
        A = new int*[row];
        for (int i = 0; i < row; i++) {
            A[i] = new int[col];
        }
    }
public:
    MATRIX(int row, int col)
    {
        init(row, col);
        zero();
    }
    ~MATRIX()
    {
        clear();
    }
    void zero()                 //將陣列中元素歸零
    {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                A[i][j] = 0;
            }
        }
    }
    int& Value(int m, int n)        //直接回傳位址，以接收值
    {
        return A[m][n];
    }
    void show()                 //輸出陣列中的值
    {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << A[i][j] << "\t";
            }
            cout << endl;
        }
    }
    void transpose()            //陣列轉換
    {
        int** tmp;              //動態記憶體宣告一個新陣列
        tmp = new int*[col];
        for (int i = 0; i < col; i++) {
            tmp[i] = new int[row];
        }
        for (int i = 0; i < col; i++) {                 //用心陣列存放旋轉過的A中的值
            for (int j = 0; j < row; j++) {
                tmp[i][j] = A[j][i];
            }
        }
        clear();                //釋放A的記憶體
        A = new int*[col];
        for (int i = 0; i < col; i++) {             //配置有旋轉過後的A的大小的新陣列A
            A[i] = new int[row];
        }
        for (int i = 0; i < col; i++) {             //將tmp中的值給A
            for (int j = 0; j < row; j++) {
                A[i][j] = tmp[i][j];
            }
        }
        for (int i = 0; i < col; i++) {             //釋放tmp記憶體
            delete [] tmp[i];
        }
        delete[]tmp;
        swap(col, row);             //交換行與列
    }
};
int main()
{
    MATRIX m(2, 3);
    m.show();
    cout << endl;
    m.Value(0, 0) = 1;
    m.Value(0, 1) = 2;
    m.Value(0, 2) = 3;
    m.Value(1, 0) = 4;
    m.Value(1, 1) = 5;
    m.Value(1, 2) = 6;
    m.show();
    cout << endl;
    m.transpose();
    m.show();
    cout << endl;
}
