#include <windows.h>
#include<vector>
#include<algorithm>
using namespace std;

vector<pair<int, int>> points;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


void MidPointLine(HDC hdc, pair<int, int> p1, pair<int, int>p2, COLORREF color) {
    int x1 = p1.first, y1 = p1.second;
    int x2 = p2.first, y2 = p2.second;


    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 >= x1) ? 1 : -1;
    int sy = (y2 >= y1) ? 1 : -1;

    int x = x1, y = y1;
    SetPixel(hdc, x, y, color);

    if (dx >= dy) {
        int d = dx - 2 * dy;
        int change1 = 2 * (dx - dy);
        int change2 = -2 * dy;

        for (int i = 0; i < dx; i++) {
            if (d <= 0) {
                y += sy;
                d += change1;
            }
            else {
                d += change2;
            }
            x += sx;
            SetPixel(hdc, x, y, color);
        }
    }
    else {
        int d = 2 * dx - dy;
        int change1 = 2 * (dx - dy);
        int change2 = 2 * dx;

        for (int i = 0; i < dy; i++) {
            if (d >= 0) {
                x += sx;
                d += change1;
            }
            else {
                d += change2;
            }
            y += sy;
            SetPixel(hdc, x, y, color);
        }
    }
}

void drawStar(HDC hdc) {
    sort(points.begin(), points.end(),
        [](pair<int, int>& p1, pair<int, int>& p2) {return p1.second > p2.second; });
    pair<int, int> middleLeftPoint, middleRightPoint
        , bottomRightPoint, bottomLeftPoint, topPoint;

    bottomLeftPoint = points[0];
    bottomRightPoint = points[1];
    if (bottomLeftPoint > bottomRightPoint) swap(bottomRightPoint, bottomLeftPoint);

    middleLeftPoint = points[2];
    middleRightPoint = points[3];

    if (middleLeftPoint > middleRightPoint) swap(middleLeftPoint, middleRightPoint);

    topPoint = points[4];

    COLORREF colors[] = {
        RGB(255, 0, 0), 
        RGB(0, 255, 0), 
        RGB(0, 0, 255), 
        RGB(0, 255, 255), 
        RGB(255, 0, 255) 
    };
    int i = 0;
    MidPointLine(hdc, topPoint, bottomLeftPoint, colors[i++]);
    MidPointLine(hdc, topPoint, bottomRightPoint, colors[i++]);
    MidPointLine(hdc, middleRightPoint, middleLeftPoint, colors[i++]);
    MidPointLine(hdc, middleRightPoint, bottomLeftPoint, colors[i++]);
    MidPointLine(hdc, middleLeftPoint, bottomRightPoint, colors[i++]);



    points.clear();

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Star");

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, TEXT("Star"), TEXT("Star Graphics Assignment"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return 0; 

    ShowWindow(hwnd, nCmdShow); 
    UpdateWindow(hwnd);


    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_LBUTTONUP:

        points.push_back({ LOWORD(lParam), HIWORD(lParam) });
        if (points.size() == 5) {
            drawStar(GetDC(hwnd));
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}