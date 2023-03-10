#include <iostream>
#include <string>
#include <cmath>
#define PI 3.14159
using namespace std;
class Point {       //存放點的座標的類別
public:
    double x;
    double y;
    Point(double x, double y) {     //將object的x, y 給值參數x, y
        this->x = x;
        this->y = y;
    }
    Point() {           
        x = 0;
        y = 0;
    }
};
class Shape {       
protected:
    string name;        //形狀名
    Point p1;           //此形狀的對角兩點
    Point p2;

public:
    Shape() {           //建構子
        
    }
    ~Shape() {          //解構子

    }
    void set(Point p1, Point p2) {      //設定此形狀的對角點
        this->p1 = p1;
        this->p2 = p2;
        
    }
    virtual double area() = 0;      
    virtual double length() = 0;

    friend ostream& operator<<(ostream &os, Shape &other) {         //輸出的 operator
        os << other.name << endl;       //輸出此 object 名稱
        os << "(" << other.p1.x << ", " << other.p1.y << ")" << ", " << "(" << other.p2.x << ", " << other.p2.y << ")" << endl;     //輸出此 object 的兩個座標
        os << "length:" << other.length() << endl;          //呼叫此 objrct 的 length function 並輸出
        os << "Area:" << other.area() << endl;              //呼叫此 objrct 的 area function 並輸出
        return os;
    }
};
class Rect : public Shape {             //繼承自類別 Shape 的子類別 Rect
public:
    Rect() {
        name = "Rectangle";             //設定此物件名稱為 Rectangle
    }
    double area() override {            //矩形的面積
        return abs(p1.x - p2.x) * abs(p1.y - p2.y);
    }
    double length() override {          //矩形的周長
        return abs(p1.x - p2.x) * 2 + abs(p1.y - p2.y) * 2;
    }
};
class Ellipse : public Shape {          //繼承自類別 Shape 的子類別 Ellipse
public:
    Ellipse() {
        name = "Ellipse";               //設定此物件名稱為 Ellipse
    }
    double area() {                     //橢圓的面積
        double a, b;
        double ans = 0;
        a = abs(p1.x - p2.x) / 2;
        b = abs(p1.y - p2.y) / 2;
        return a * b * PI;
    }
    double length() {                   //橢圓的周長
        double c;
        double a, b;
        double ans = 0;
        a = abs(p1.x - p2.x) / 2;
        b = abs(p1.y - p2.y) / 2;
        c = sqrt(abs(pow(a, 2) - pow(b, 2)));
        double tmp = 1;
        double tmp2 = 1;
        for (int i = 1;i<1000; i+=2) {
            tmp2 = tmp2 * (i / (i + 1));
            tmp -= pow(tmp2, 2) * pow(c, i + 1) / (i * pow(a, i + 1));
        }
        ans = 2 * PI * a * tmp;
        return ans;
    }
};


int main()
{
    Shape* s = new Rect();
    s->set(Point(1, 1), Point(2, 2));
    cout << *s << endl;
    delete s;
    s = new Ellipse();
    s->set(Point(0, 0), Point(2, 2));
    cout << *s << endl;
    delete s;
}
