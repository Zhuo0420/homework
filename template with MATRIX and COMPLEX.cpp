#include <iostream>
#include <vector>   
#include <iomanip>
using namespace std;

template<class T>
class MATRIX {
private:
    int col = 0;
    int row = 0;
    vector<vector<T>> matrix;
public:
    MATRIX() {
        col = 0;
        row = 0;
    }
    MATRIX(int col, int row) {
        this->col = col;
        this->row = row;
        init(col, row);
    }
    void init(int col, int row) {
        this->col = col;
        this->row = row;
        this->matrix.resize(col);
        for (auto& i : matrix) {
            i.resize(row);
        }
    }
    void inv(MATRIX& inverse) {
        MATRIX tmp((inverse.col) - 1, (inverse.row) - 1);
        
        for (int i = 0; i < inverse.col; i++) {
            for (int j = 0; j < inverse.row; j++) {
                for (int k = 0; k < inverse.col; k++) {
                    if (i != k && j != k) {
                        if ((inverse.col)-1 == 2) {

                        }
                    }
                }
            }
        }
    }
    MATRIX& operator=(const MATRIX& other) {
        init(other.col, other.row);
        for (int i = 0; i < other.col; i++) {
            for (int j = 0; j < other.row; j++) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
        return *this;
    }

    friend istream& operator>>(istream& is, MATRIX& other) {
        for (int i = 0; i < other.col; i++) {
            for (int j = 0; j < other.row; j++) {
                is >> other.matrix[i][j];
            }
        }
        return is;
    }
    friend ostream& operator<<(ostream& os, const MATRIX& other) {
        for (int i = 0; i < other.col; i++) {
            for (int j = 0; j < other.row; j++) {
                os <<other.matrix[i][j] <<"  ";
            }
            os << endl;
        }
        return os;
    }
    MATRIX operator+(MATRIX& other) {
        MATRIX result(col, row);
        if (col == other.col && row == other.row) {
            for (int i = 0; i < col; i++) {
                for (int j = 0; j < row; j++) {
                    result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
                }
            }
        }
        return result;
    }
    MATRIX operator-(MATRIX& other) {
        MATRIX result(col, row);
        if (col == other.col && row == other.row) {
            for (int i = 0; i < col; i++) {
                for (int j = 0; j < row; j++) {
                    result.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
                }
            }
        }
        return result;

    }
    MATRIX operator*(MATRIX other) {
        MATRIX result(col, other.row);
        if (row == other.col) {
            for (int i = 0; i < col; i++) {
                for (int j = 0; j < row; j++) {
                    for (int k = 0; k < col; k++) {
                        result.matrix[i][j] = result.matrix[i][j] + (matrix[i][k] * other.matrix[k][j]);
                    }                    
                }
            }
        }
        return result;
    }
    MATRIX operator/(MATRIX& other) {
        MATRIX result(col, other.row);
        if (col == other.row) {
            MATRIX inverse(col, other.row);
            inverse = 
        }
        
        return result;
    }
};

template<class T>
class COMPLEX {
private:
    T im;
    T re;       
public:
    COMPLEX() {
        re = 0;
        im = 0;
    }
    COMPLEX& operator=(COMPLEX other) {
        re = other.re;
        im = other.im;
        return *this;
    }
    friend istream& operator>>(istream& is, COMPLEX& other) {
        is >> other.re;
        is >> other.im;
        return is;
    }
    friend ostream& operator<<(ostream& os, const COMPLEX& other) {
        if (other.re == 0) {
            os << setw(3) << other.im << 'i';
        }
        else {
            os << other.re << "+";
            os << other.im << "i";;
        }
        return os;
    }
    COMPLEX operator+(COMPLEX other) {
        COMPLEX tmp;
        tmp.re = re + other.re;
        tmp.im = im + other.im;
        return tmp;
    }
    COMPLEX operator-(COMPLEX other) {
        COMPLEX tmp;
        tmp.re = re - other.re;
        tmp.im = im - other.im;
        return tmp;
    }
    COMPLEX operator*(COMPLEX other) {
        COMPLEX tmp;
        tmp.im = (re * other.re - im * other.im) + (im * other.re + re * other.im);
        return tmp;
    }

};

int main()
{
    MATRIX<COMPLEX<double>> A(3, 3), B(3, 3), C, D;
    cin >> A >> B;
    C = A * B;
    D = C / A;
    cout << A << endl;
    cout << B << endl;
    cout << C << endl;
    cout << D << endl;
    //MATRIX<int> tmp;
}
