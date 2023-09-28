#include <windows.h>
#include <iostream>
#include <ctime>

bool start = true;
bool play = false;
const int size = 150;

int cells[size][size];

int neighbourSum(int xStart, int yStart) {
    int sum = 0;
    for (int x = xStart - 1; x <= xStart + 1; x++) {
        if (x < 0) {
            x = 0;
        }
        for (int y = yStart - 1; y <= yStart + 1; y++) {
            if (y < 0) {
                y = 0;
            }
            sum += cells[x][y];
        }
    }
    sum -= cells[xStart][yStart];
    return sum;
};

void randCells() {
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            int random = rand() % 20;
            if (random == 0) {
                random = 1;
            }
            else {
                random = 0;
            }
            cells[x][y] = random;
        }
    }
}

void checkMatrix() {
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            int condition = neighbourSum(x, y);
            if (cells[x][y] == 0) {
                if (condition == 3) {
                    cells[x][y] = 1;
                }
            }
            else {
                if (condition > 3 || condition < 2) {
                    cells[x][y] = 0;
                }
            }
        }
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        HDC hdc = GetDC(hwnd);
        if (start) {
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hbr = CreateSolidBrush(RGB(0, 8, 61));
            FillRect(hdc, &rect, hbr);
            DeleteObject(hbr);
            start = false;
        }
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                COLORREF color;
                if (cells[x][y] == 1) {
                    color = RGB(255, 255, 255);
                }
                else {
                    color = RGB(0, 0, 0);
                }
                HBRUSH hbr = CreateSolidBrush(color);
                SelectObject(hdc, hbr);
                int xCoor = 100 + x * 600 / size;
                int yCoor = 100 + y * 600 / size;
                Rectangle(hdc, xCoor, yCoor, xCoor + 600 / size, yCoor + 600 / size);
                DeleteObject(hbr);
            }
        }
        checkMatrix();
        while (!play) {
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                play = true;
                Sleep(100);
            }
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            play = !play;
            Sleep(100);
        }
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            randCells();
            start = true;
            play = false;
        }

        ReleaseDC(hwnd, hdc);

        return 0;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int main() {
    srand(time(0));
    randCells();
    // Create a window class
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = TEXT("MyWindowClass");
    RegisterClass(&wc);

    // Create a window
    HWND hwnd = CreateWindowEx(0, TEXT("MyWindowClass"), TEXT("My Window"), WS_OVERLAPPEDWINDOW,
        0, 0, 800, 800, NULL, NULL, GetModuleHandle(NULL), NULL);

    // Show the window
    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    BOOL bRet;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
        if (bRet == -1) {
            // Handle errors here
            break;
        }
        else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}