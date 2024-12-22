#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER "abcdefghijklmnopqrstuvwxyz"
#define DIGITS "0123456789"
#define SYMBOLS "!@#$%^&*()-_=+[]{}|;:,.<>?/"

char password[256] = ""; 
HWND hLength, hUpper, hLower, hDigits, hSymbols, hGenerate, hOutput;

void generate_password(int length, int use_upper, int use_lower, int use_digits, int use_symbols) {
    char charset[256] = "";
    strcpy(password, "");

    if (use_upper) strcat(charset, UPPER);
    if (use_lower) strcat(charset, LOWER);
    if (use_digits) strcat(charset, DIGITS);
    if (use_symbols) strcat(charset, SYMBOLS);

    int charset_length = strlen(charset);
    if (charset_length == 0) {
        strcpy(password, "Error: No character types selected!");
        return;
    }

    for (int i = 0; i < length; i++) {
        password[i] = charset[rand() % charset_length];
    }
    password[length] = '\0';
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            HFONT hFont = CreateFont(
                20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Arial");

            CreateWindow("STATIC", "Length:",
                WS_VISIBLE | WS_CHILD, 30, 30, 100, 20,
                hwnd, NULL, NULL, NULL);
            hLength = CreateWindow("EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                140, 30, 100, 25, hwnd, NULL, NULL, NULL);
            SendMessage(hLength, WM_SETFONT, (WPARAM)hFont, TRUE);

            hUpper = CreateWindow("BUTTON", "Uppercase",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                30, 70, 120, 25, hwnd, NULL, NULL, NULL);
            SendMessage(hUpper, WM_SETFONT, (WPARAM)hFont, TRUE);

            hLower = CreateWindow("BUTTON", "Lowercase",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                30, 100, 120, 25, hwnd, NULL, NULL, NULL);
            SendMessage(hLower, WM_SETFONT, (WPARAM)hFont, TRUE);

            hDigits = CreateWindow("BUTTON", "Digits",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                30, 130, 120, 25, hwnd, NULL, NULL, NULL);
            SendMessage(hDigits, WM_SETFONT, (WPARAM)hFont, TRUE);

            hSymbols = CreateWindow("BUTTON", "Symbols",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                30, 160, 120, 25, hwnd, NULL, NULL, NULL);
            SendMessage(hSymbols, WM_SETFONT, (WPARAM)hFont, TRUE);

            hGenerate = CreateWindow("BUTTON", "Generate",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                30, 200, 100, 30, hwnd, (HMENU)1, NULL, NULL);
            SendMessage(hGenerate, WM_SETFONT, (WPARAM)hFont, TRUE);


            hOutput = CreateWindow("EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_MULTILINE,
                30, 250, 300, 50, hwnd, NULL, NULL, NULL);
            SendMessage(hOutput, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) { 
                char lengthBuffer[10];
                int length, use_upper, use_lower, use_digits, use_symbols;

                GetWindowText(hLength, lengthBuffer, 10);
                length = atoi(lengthBuffer);

                use_upper = SendMessage(hUpper, BM_GETCHECK, 0, 0);
                use_lower = SendMessage(hLower, BM_GETCHECK, 0, 0);
                use_digits = SendMessage(hDigits, BM_GETCHECK, 0, 0);
                use_symbols = SendMessage(hSymbols, BM_GETCHECK, 0, 0);

                generate_password(length, use_upper, use_lower, use_digits, use_symbols);
                SetWindowText(hOutput, password);
            }
            break;
        }
        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLOREDIT: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0)); 
            SetBkColor(hdcStatic, RGB(240, 240, 240)); 
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG messages;
    WNDCLASS wincl = {0};

    wincl.hInstance = hInstance;
    wincl.lpszClassName = "PasswordGenerator";
    wincl.lpfnWndProc = WindowProcedure;
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wincl)) return 0;

    HWND hwnd = CreateWindow(
        "PasswordGenerator", "Password Generator",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 350,
        HWND_DESKTOP, NULL, hInstance, NULL
    );

    srand(time(NULL));

    while (GetMessage(&messages, NULL, 0, 0)) {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}
