// 20230317 視窗程式設計.cpp : 定義應用程式的進入點。
//

#include "framework.h"
#include "20230317 視窗程式設計.h"
#include <vector>
using namespace std;

#define MAX_LOADSTRING 100

class point {       //存放點的座標的類別
public:
    int x;
    int y;
    point(int x, int y) {     //將object的x, y 給值參數x, y
        this->x = x;
        this->y = y;
    }
    point() {
        x = 0;
        y = 0;
    }
    
};
class SHAP {
protected:
    point p1;           //此形狀的對角兩點
    point p2;

public:
    SHAP() {
    }

    SHAP(point p1, point p2) {
    }
    void set(point p1, point p2) {      //設定此形狀的對角點
        this->p1 = p1;
        this->p2 = p2;
    }
    virtual void draw(HDC hdc) {

    }
};
class RECTANGLE : public SHAP {             //類別RECTANGLE繼承自類別SHAP
public:
    RECTANGLE() {

    }
    RECTANGLE(point p1, point p2) {         //設定此形狀對角點
        set(p1, p2);
    }
    void draw(HDC hdc) override {           
        Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);     //呼叫Rectangle函數並將參數的筆刷路徑傳入，畫出矩形
    }
};
class ELLIPSE : public SHAP{
public:
    ELLIPSE(point p1, point p2) {
        set(p1, p2);
    }
    void draw(HDC hdc) override {           
        Ellipse(hdc, p1.x, p1.y, p2.x, p2.y);       //呼叫Rectangle函數並將參數的筆刷路徑傳入，畫出橢圓
    }
    
};
class LINE : public SHAP {
public:
    LINE(point p1, point p2) {
        set(p1, p2);
    }
    void draw(HDC hdc) override {       

        MoveToEx(hdc, p1.x, p1.y, 0);           //呼叫Rectangle函數並將參數的筆刷路徑傳入(線的起點)
        LineTo(hdc, p2.x, p2.y);                //呼叫Rectangle函數並將參數的筆刷路徑傳入(線的終點)
    }
};


// 全域變數:
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱

// 這個程式碼模組所包含之函式的向前宣告:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置程式碼。

    // 將全域字串初始化
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY20230317, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 執行應用程式初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY20230317));

    MSG msg;

    // 主訊息迴圈:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函式: MyRegisterClass()
//
//  用途: 註冊視窗類別。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20230317));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY20230317);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函式: InitInstance(HINSTANCE, int)
//
//   用途: 儲存執行個體控制代碼並且建立主視窗
//
//   註解:
//
//        在這個函式中，我們將執行個體控制代碼儲存在全域變數中，
//        並建立及顯示主程式視窗。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函式: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  用途: 處理主視窗的訊息。
//
//  WM_COMMAND  - 處理應用程式功能表
//  WM_PAINT    - 繪製主視窗
//  WM_DESTROY  - 張貼結束訊息然後傳回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static vector<SHAP*> shapes;
    switch (message)
    {
    case WM_CREATE: {
        shapes.push_back(new RECTANGLE(point(300, 150), point(500, 350)));      //畫方型
        int a = 300, b = 150;
        for (int i = 0; i < 50; i++) {      //用為圈疊加，形成黑色實心的效果
            shapes.push_back(new RECTANGLE(point(a++, b++), point(a+200, b+200)));
        }
        shapes.push_back(new LINE(point(300, 150), point(350, 200)));       //畫線
        shapes.push_back(new LINE(point(500, 150), point(550, 200)));
        shapes.push_back(new LINE(point(300, 350), point(350, 400)));
        shapes.push_back(new RECTANGLE(point(350, 200), point(550, 400)));
        shapes.push_back(new ELLIPSE(point(400, 250), point(450, 300)));        //畫橢圓
        shapes.push_back(new ELLIPSE(point(490, 250), point(540, 300)));
        shapes.push_back(new ELLIPSE(point(420, 350), point(540, 390)));
        shapes.push_back(new LINE(point(370, 320), point(390, 340)));
        shapes.push_back(new LINE(point(380, 320), point(400, 340)));
        shapes.push_back(new LINE(point(390, 320), point(410, 340)));
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 剖析功能表選取項目:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            for (SHAP* s : shapes) {
                s->draw(hdc);       //依序呼叫shape中存入的位址指向的類別的draw函數，將s中所存的筆刷路徑路徑作為參數傳入
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        for (SHAP* s : shapes)      
            delete s;               ////依序呼叫shape中存入的位址指向的類別的draw函數，並釋放記憶體
        shapes.clear();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// [關於] 方塊的訊息處理常式。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
