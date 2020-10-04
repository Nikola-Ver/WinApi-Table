#include <Windows.h>
#include <fstream>
#include "lab_2.h"

#define PADDING 5

const int lenCol = 6, lenRow = 18;
const wchar_t *list[lenRow][lenCol] = {
                                        { L"text one one one one one", L"text two two two two two text two two two two two", L"text three three three three" , L"text three three three three" , L"text three three three three" , L"text three three three three" },
                                        { L"text three three three three", L"text one one one one one", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text three three three three", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text two two two two two text two two two two two", L"text three three three three" , L"text three three three three" , L"text three three three three" , L"text three three three three" },
                                        { L"text three three three three", L"text one one one one one", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text three three three three", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text two two two two two text two two two two two", L"text three three three three" , L"text three three three three" , L"text three three three three" , L"text three three three three" },
                                        { L"text three three three three", L"text one one one one one", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text three three three three", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text two two two two two text two two two two two", L"text three three three three" , L"text three three three three" , L"text three three three three" , L"text three three three three" },
                                        { L"text three three three three", L"text one one one one one", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text three three three three", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text two two two two two text two two two two two", L"text three three three three" , L"text three three three three" , L"text three three three three" , L"text three three three three" },
                                        { L"text three three three three", L"text one one one one one", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text three three three three", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text two two two two two text two two two two two", L"text three three three three" , L"text three three three three" , L"text three three three three" , L"text three three three three" },
                                        { L"text three three three three", L"text one one one one one", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" },
                                        { L"text one one one one one", L"text three three three three", L"text two two two two two", L"text two two two two two", L"text two two two two two", L"text two two two two two" }
                                      };
RECT rcSize;
HDC hdcBackBuffer, hdcTable;
int currentInternalHeight = 0;

bool DrawLine(HDC hdc, POINT ptFrom, POINT ptTo)
{
    POINT pt;
    int   x = ptFrom.x, 
          y = ptFrom.y;
    MoveToEx(hdc, x, y, &pt);
    return LineTo(hdc, ptTo.x, ptTo.y);
}

VOID DrawTable(HDC hdc)
{
    FillRect(hdc, &rcSize, (HBRUSH)GetStockObject(WHITE_BRUSH));
    int sizeCol = (int)((rcSize.right - rcSize.left) / lenCol);
    RECT layoutText;
    layoutText.top = currentInternalHeight + PADDING;

    POINT ptFrom, ptTo;
    ptFrom.x = sizeCol;
    ptFrom.y = currentInternalHeight;
    ptTo.x = sizeCol;
    ptTo.y = currentInternalHeight; 

    /* Draw rows with text */
    for (int i = 0; i < lenRow ; i++)
    {
        layoutText.left = PADDING;
        int maxHeightRow = 0;

        for (int j = 0; j < lenCol; j++)
        {
            int heightRow;
            layoutText.left = ptFrom.x - sizeCol + PADDING;
            layoutText.right = ptFrom.x - PADDING;
            RECT rect;
            rect.left = ptFrom.x;
            
            heightRow = DrawText(hdc, list[i][j], wcslen(list[i][j]), &layoutText, DT_CALCRECT | DT_WORDBREAK);
            layoutText.bottom = layoutText.top + heightRow;
            DrawText(hdc, list[i][j], wcslen(list[i][j]), &layoutText, DT_WORDBREAK);
            
            maxHeightRow = heightRow > maxHeightRow ? heightRow : maxHeightRow;

            ptFrom.x += sizeCol;
            ptTo.x += sizeCol;
        }
        layoutText.top += maxHeightRow + PADDING;
        
        ptFrom.x = 0;
        ptFrom.y += maxHeightRow + PADDING;
        ptTo.x = rcSize.right - rcSize.left;
        ptTo.y += maxHeightRow + PADDING;
        DrawLine(hdc, ptFrom, ptTo);

        ptFrom.x = sizeCol;
        ptTo.x = sizeCol;
    }
    ptFrom.y = 0;

    /* Draw columns */
    for (int i = 0; i < lenCol - 1; i++)
    {
        DrawLine(hdc, ptFrom, ptTo);
        ptFrom.x += sizeCol;
        ptTo.x = ptFrom.x;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (msg)
    {
    case WM_MOUSEWHEEL:
    {
        InvalidateRect(hWnd, &rcSize, true);
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (currentInternalHeight + delta <= 0)
            currentInternalHeight += delta;

        return 0;
    }
    case WM_SIZE: 
    {
        HDC hdcWindow = GetDC(hWnd);

        GetClientRect(hWnd, &rcSize);
        InvalidateRect(hWnd, &rcSize, true);
        if (hdcBackBuffer) DeleteDC(hdcBackBuffer);
        hdcBackBuffer = CreateCompatibleDC(hdcWindow);
        HBITMAP hbmBackBuffer = CreateCompatibleBitmap(hdcBackBuffer, rcSize.right - rcSize.left, rcSize.bottom - rcSize.top);
        SelectObject(hdcBackBuffer, hbmBackBuffer);
        DeleteObject(hbmBackBuffer);

        if (hdcTable) DeleteDC(hdcTable);
        hdcTable = CreateCompatibleDC(hdcWindow);
        HBITMAP hbmTable = CreateCompatibleBitmap(hdcTable, rcSize.right - rcSize.left, rcSize.bottom - rcSize.top);
        SelectObject(hdcTable, hbmTable);
        DeleteObject(hbmTable);

        ReleaseDC(hWnd, hdcWindow);
        return 0;
    }
    case WM_CREATE:
    {
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        DrawTable(hdcTable);
        BitBlt(hdcBackBuffer, 0, 0, rcSize.right - rcSize.left, rcSize.bottom - rcSize.top, hdcTable, 0, 0, SRCCOPY);
        BitBlt(ps.hdc, 0, 0, rcSize.right - rcSize.left, rcSize.bottom - rcSize.top, hdcBackBuffer, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        return 0;
    default:
    {
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    }
}

int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR className[] = TEXT("GameClass");
    static TCHAR windowName[] = TEXT("WinApi");

    WNDCLASSEX wcex;

    wcex.cbClsExtra = 0;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbWndExtra = 0;
    wcex.hbrBackground = NULL;
    wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hIconSm = NULL;
    wcex.hInstance = hInstance;
    wcex.lpfnWndProc = WndProc;
    wcex.lpszClassName = className;
    wcex.lpszMenuName = NULL;
    wcex.style = 0;

    if (!RegisterClassEx(&wcex))
        return 0;

    HWND hWnd = CreateWindow(className, windowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, NULL, hInstance, NULL);
    if (!hWnd)
        return 0;

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return msg.wParam;
}