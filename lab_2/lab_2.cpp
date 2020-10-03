#include <Windows.h>
#include <fstream>
#include "lab_2.h"

//std::string* list = NULL;
std::string list[3][3] = { 
                            { {"text one one one one one"}, {"text two two two two two"}, {"text three three three three"} }, 
                            { {"text three three three three"}, {"text one one one one one"}, {"text two two two two two"} },
                            { {"text one one one one one"}, {"text three three three three"}, {"text two two two two two"} }
                          };
int  lenRow = 3, lenCol = 3;
RECT rcSize;

//VOID ReadFile()
//{
//    std::string line;
//    std::ifstream file;
//    file.open("table.txt");
//
//    while (file >> line) lenList++;
//    list = new std::string[lenList];
//    file.seekg(0, file.beg);
//    for (int i = 0; i < lenList; i++) file >> list[i];
//
//    file.close();
//}

bool DrawLine(HDC hdc, POINT ptFrom, POINT ptTo)
{
    POINT pt;
    int   x = ptFrom.x, 
          y = ptFrom.y;
    MoveToEx(hdc, x, y, &pt);
    return LineTo(hdc, ptTo.x, ptTo.y);
}

VOID DrawTable(HDC hdc, RECT rcHwnd, std::string **listOfText[100], int _lenRow, int _lenCol)
{
    int sizeCol = (int)((rcSize.right - rcSize.left) / _lenCol);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (msg)
    {
    case WM_SIZE: 
    {
        GetClientRect(hWnd, &rcSize);
        return 0;
    }
    case WM_CREATE:
    {
        return 0;
    }
    case WM_DESTROY:
    {
        //delete[](list);
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawTable(hdc, rcSize, list, lenRow, lenCol);
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