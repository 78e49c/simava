#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Array to map random numbers to characters
char charArray[] = {'P', 'V', 'T', 'S', 'L', 'M', 'K', 'Y'};

// Function to convert a random number to a character
char convertToChar(int num) {
    return charArray[num];
}

// Handle for the input edit control
HWND hEdit;
// Handle for the output static control
HWND hOutput;

// Main entry point for the program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SimavaWindow";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, "SimavaWindow", "Random Character Generator", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 350, 120,
        NULL, NULL, hInstance, NULL);

    // Create the input edit control
    hEdit = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 10, 10, 50, 20, hwnd, NULL, hInstance, NULL);
    // Create the "Generate" button
    HWND hButton = CreateWindow("BUTTON", "Generate", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 70, 10, 70, 20, hwnd, NULL, hInstance, NULL);
    // Create the output static control
    hOutput = CreateWindow("STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT, 10, 50, 300, 20, hwnd, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Window procedure to handle messages for the main window
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            // Handle button clicks
            if (HIWORD(wParam) == BN_CLICKED) {
                int numChars;
                char output[100];

                // Get the text from the input edit control
                GetWindowText(hEdit, output, 100);
                numChars = atoi(output);

                if (numChars > 0) {
                    HDC hdc = GetDC(hOutput);
                    RECT rect;
                    GetClientRect(hOutput, &rect);
                    // Clear the output area
                    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

                    srand(time(NULL)); // Set seed for random number generation

                    for (int i = 0; i < numChars; i++) {
                        int randomValue = rand() % 8; // Generate a random number between 0 and 7
                        output[i] = convertToChar(randomValue);
                    }
                    output[numChars] = '\0'; // Null-terminate the string

                    // Display the generated characters in the output area
                    SetWindowText(hOutput, output);

                    ReleaseDC(hOutput, hdc);
                }
            }
            break;
        case WM_DESTROY:
            // Handle window destruction
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
