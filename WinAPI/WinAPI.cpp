#ifndef UNICODE
#define UNICODE
#endif 

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include "Resource.h"
#include <commctrl.h>
#include <Windows.h>
#include <winuser.h>
#include <windowsx.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HACCEL hAccel;
HIMAGELIST himl = ImageList_Create(120, 66, ILC_COLOR32, 1, 0);;
bool isDragging = false;
bool isMoving = false;
int picture_x;
int picture_y;
int PICTURE_WIDTH;
int PICTURE_HEIGHT;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 630;
int DELTA_X = 5;
int DELTA_Y = 5;
int mouseX;
int mouseY;


bool AddIconsToImageList(HINSTANCE hinst, HIMAGELIST list, int x, int y)
{ 
    picture_x = x;
    picture_y = y;
    BITMAP bmp;
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, 
        L"C:\\Users\\Пользователь\\Downloads\\dvd.bmp", 
        IMAGE_BITMAP, x, y, LR_LOADFROMFILE);
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    PICTURE_WIDTH = bmp.bmWidth;
    PICTURE_HEIGHT = bmp.bmHeight;
    ImageList_Add(list, hBitmap, NULL);
    int imageIndex = ImageList_Add(list, hBitmap, NULL);
    DeleteObject(hBitmap);
    if (imageIndex != -1) return true; else return false;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    WNDCLASS wc = { };
    hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI));
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(
        0,                                // Optional window styles.
        CLASS_NAME,                       // Window class
        L"Time killer",                   // Window text
        WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX,// Window style
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
    if (hwnd == NULL)
    {
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (!TranslateAccelerator(hwnd, hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}

void PaintImage(HWND hwnd, HDC hdc, int x, int y) {
    int imageIndex = 0;
    ImageList_Draw(himl, imageIndex, hdc, x, y, ILD_NORMAL);
}

void MoveImage(int deltaX, int deltaY) {
    if (!isDragging) {
        if (deltaX > 0) {
            if (picture_x + deltaX + PICTURE_WIDTH >= 800) {
                picture_x = 770 - PICTURE_WIDTH;
            }
            else {
                picture_x += deltaX;
            }
        }
        else {
            if (picture_x + deltaX < 0) {
                picture_x = 30;
            }
            else {
                picture_x += deltaX;
            }
        }

        if (deltaY > 0) {
            if (picture_y + deltaY >= 600 - PICTURE_HEIGHT) {
                picture_y = 570 - PICTURE_HEIGHT;
            }
            else {
                picture_y += deltaY;
            }
        }
        else {
            if (picture_y + deltaY < 0) {
                picture_y = 30;
            }
            else {
                picture_y += deltaY;
            }
        }
    }
    else {
        picture_x += deltaX;
        picture_y += deltaY;
    }
}

void UpdateImagePosition(HWND hwnd) {
    if (picture_x + DELTA_X + PICTURE_WIDTH > 790) {
        DELTA_X *= -1;
    }
    if (picture_x + DELTA_X < 0) {
        DELTA_X *= -1;
    } 
    if (picture_y + DELTA_Y > 590 - PICTURE_HEIGHT) {
        DELTA_Y *= -1;
    }
    if (picture_y + DELTA_Y < 0) {
        DELTA_Y *= -1;
    }
    picture_x += DELTA_X;
    picture_y += DELTA_Y;
    InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    wchar_t msg[32];
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
    {
        AddIconsToImageList(NULL, himl, 0, 0);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        FillRect(hdc, &clientRect, hBrush);
        //FillRect(hdc, &ps.rcPaint, (HBRUSH)RGB(0, 0, 0));
        PaintImage(hwnd, hdc, picture_x, picture_y);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        if (isMoving == false) {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            if ((xPos >= picture_x && xPos <= picture_x + PICTURE_WIDTH) &&
                (yPos >= picture_y && yPos <= picture_y + PICTURE_HEIGHT)) {
                isDragging = true;
                mouseX = xPos;
                mouseY = yPos;
            }
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        if (isMoving == false) {
            isDragging = false;
        }
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (isDragging && isMoving == false) {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            int dx = xPos - mouseX;
            int dy = yPos - mouseY;
            picture_x += dx;
            picture_y += dy;
            mouseX = xPos;
            mouseY = yPos;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    }
    case WM_KEYDOWN:
    {
        if (isMoving == false) {
            switch (wParam) {
            case VK_LEFT:
                MoveImage(-10, 0); // Сдвигаем изображение влево
                break;
            case VK_RIGHT:
                MoveImage(10, 0); // Сдвигаем изображение вправо
                break;
            case VK_UP:
                MoveImage(0, -10); // Сдвигаем изображение влево
                break;
            case VK_DOWN:
                MoveImage(0, 10); // Сдвигаем изображение вправо
                break;
            }
            InvalidateRect(hwnd, NULL, TRUE);
        }
    }
    case WM_MOUSEWHEEL:
    {
        if (isMoving == false) {
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (!isDragging) {
                if (GetKeyState(VK_SHIFT) < 0 && delta != 0) {
                    delta > 0 ? MoveImage(-10, 0) : MoveImage(10, 0);
                }
                else if (delta != 0) {
                    delta > 0 ? MoveImage(0, -10) : MoveImage(0, 10);
                }
            }
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    }
    case WM_TIMER:
    {
        UpdateImagePosition(hwnd);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_MOVING:
        {
            if (isMoving) {
                KillTimer(hwnd,1);
                isMoving = false;
            }
            else {
                SetTimer(hwnd, 1, 100, NULL);
                isMoving = true;
            }
            break;
        }
        }
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}