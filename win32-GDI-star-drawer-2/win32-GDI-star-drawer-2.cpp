#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

vector<pair<int, int>> points;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void MidPointLine(HDC hdc, pair<int, int> p1, pair<int, int> p2, COLORREF color) {
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
    pair<int, int> leftMost = *min_element(points.begin(), points.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return (a.first < b.first) || (a.first == b.first && a.second < b.second);
     });

    int x_ref = leftMost.first, y_ref = leftMost.second;

    sort(points.begin(), points.end(), [x_ref, y_ref](const pair<int, int>& a, const pair<int, int>& b) {
        double slopeA = (a.first == x_ref) ? INFINITY : (double)(a.second - y_ref) / (a.first - x_ref);
        double slopeB = (b.first == x_ref) ? INFINITY : (double)(b.second - y_ref) / (b.first - x_ref);
        return slopeA < slopeB;
      });


    pair<int, int> star[6];
    star[0] = points[0];
    star[1] = points[2];
    star[2] = points[4];
    star[3] = points[1];
    star[4] = points[3];
    star[5] = star[0]; 

    
    
    COLORREF colors[] = {
        RGB(255, 0, 0),
        RGB(0, 255, 0),
        RGB(0, 0, 255),
        RGB(0, 255, 255),
        RGB(255, 0, 255)
    };
    int j = 0;
    
    for (int i = 0; i < 5; i++, j++) {
        MidPointLine(hdc, star[i], star[i + 1], colors[j]);
    }

    points.clear();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Star");
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

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
