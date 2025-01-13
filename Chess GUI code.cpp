#include "framework.h"
#include <windows.h>
#include <map>       // Include map
#include <string>    // Include string
#include "chess GUI.h"
#include <vector>
#include <string>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


#define MAX_LOADSTRING 100
#define BOARD_SIZE 8
#define CELL_SIZE 70 // Size of each square on the board

std::map<std::wstring, std::wstring> pieceArt = {
    {L"PB", L"♟️"}, {L"PW", L"♙"},
    {L"RB", L"♜"}, {L"RW", L"♖"},
    {L"NB", L"♞"}, {L"NW", L"♘"},
    {L"BB", L"♝"}, {L"BW", L"♗"},
    {L"QB", L"♛"}, {L"QW", L"♕"},
    {L"KB", L"♚"}, {L"KW", L"♔"}
};

// Global Variables
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
POINT whiteKingPos = { 4, 7 }; // Initially on E1 (0-indexed)
POINT blackKingPos = { 4, 0 }; // Initially on E8 (0-indexed)

HWND hWndMain; // Handle to the main window

// Chessboard and pieces data
std::vector<std::wstring> chessboard(BOARD_SIZE* BOARD_SIZE, L""); // Board with pieces
POINT selectedPiece = { -1, -1 }; // Selected piece coordinates
bool playerTurn = false; // 0 for player 1 (white), 1 for player 2 (black)
std::vector<POINT> validMoves;

// Forward declarations
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawChessboard(HDC hdc);
void InitializeChessboard();
bool ValidMove(POINT from, POINT to);
void MovePiece(POINT from, POINT to);
void DrawPiece(HDC hdc, int row, int col, const std::wstring& piece);
void TurnChange(bool& turn);
bool IsPlayerPiece(POINT position);

// Entry Point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CHESSGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHESSGUI));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHESSGUI));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CHESSGUI);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;

    hWndMain = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CELL_SIZE * BOARD_SIZE + 20, CELL_SIZE * BOARD_SIZE + 40,
        nullptr, nullptr, hInstance, nullptr);

    if (!hWndMain) {
        return FALSE;
    }

    InitializeChessboard(); // Set up initial pieces

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    return TRUE;
}

void InitializeChessboard() {
    // Place initial pieces
    std::wstring initialRow1 = L"RNBQKBNR"; // Rook, Knight, Bishop, Queen, King
    std::wstring initialRow2 = L"PPPPPPPP"; // Pawns

    // Place black pieces
    for (int i = 0; i < BOARD_SIZE; ++i) {
        chessboard[i] = std::wstring(1, initialRow1[i]) + L"B";
        chessboard[BOARD_SIZE + i] = std::wstring(1, initialRow2[i]) + L"B";
    }

    // Empty rows
    for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            chessboard[i * BOARD_SIZE + j] = L"";
        }
    }

    // Place white pieces
    for (int i = 0; i < BOARD_SIZE; ++i) {
        chessboard[(6 * BOARD_SIZE) + i] = std::wstring(1, initialRow2[i]) + L"W";
        chessboard[(7 * BOARD_SIZE) + i] = std::wstring(1, initialRow1[i]) + L"W";
    }
}

// Add this function to draw the turn message

bool IsMoveCapturingKing(POINT to) {
    std::wstring piece = chessboard[to.y * BOARD_SIZE + to.x];
    return piece[0] == L'K';
}

void DrawTurnMessage(HDC hdc) {
    // Set up the font for the message
    HFONT hFont = CreateFont(
        30,               // Height of the font
        0,                // Width of the font
        0,                // Angle of escapement
        0,                // Orientation angle
        FW_BOLD,          // Font weight
        FALSE,            // Italic
        FALSE,            // Underline
        FALSE,            // Strikeout
        DEFAULT_CHARSET,  // Character set
        OUT_DEFAULT_PRECIS, // Output precision
        CLIP_DEFAULT_PRECIS, // Clipping precision
        DEFAULT_QUALITY,    // Output quality
        DEFAULT_PITCH | FF_SWISS, // Pitch and family
        L"Arial"          // Font name
    );

    // Select the custom font into the HDC
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    // Set background transparency and text color
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0)); // Black text color

    // Display the message based on the current player's turn
    std::wstring turnMessage = playerTurn ? L"Black's Turn" : L"White's Turn";
    RECT messageRect = { 0, CELL_SIZE * BOARD_SIZE + 10, CELL_SIZE * BOARD_SIZE, CELL_SIZE * BOARD_SIZE + 40 };
    DrawTextW(hdc, turnMessage.c_str(), -1, &messageRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Restore the old font and delete the custom font
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

void CalculateValidMoves(POINT from) {
    validMoves.clear();
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            POINT to = { x, y };
            if (ValidMove(from, to)) {
                validMoves.push_back(to);
            }
        }
    }
}

void DrawChessboard(HDC hdc) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            RECT cell = { col * CELL_SIZE, row * CELL_SIZE, (col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE };
            HBRUSH brush;

            bool isValidMove = false;
            for (const auto& move : validMoves) {
                if (move.x == col && move.y == row) {
                    isValidMove = true;
                    break;
                }
            }

            if (isValidMove) {
                brush = CreateSolidBrush(RGB(255, 185, 120));
            }
            else {
                brush = CreateSolidBrush((row + col) % 2 == 0 ? RGB(240, 217, 181) : RGB(181, 136, 99));
            }

            FillRect(hdc, &cell, brush);
            DeleteObject(brush);

            std::wstring piece = chessboard[row * BOARD_SIZE + col];
            if (!piece.empty()) {
                DrawPiece(hdc, row, col, piece);
            }
        }
    }

    // Draw the turn message below the board
    DrawTurnMessage(hdc);
}

void DrawPiece(HDC hdc, int row, int col, const std::wstring& piece) {
    if (pieceArt.find(piece) == pieceArt.end()) return; // Skip empty squares

    // Create a larger font for the chess pieces
    HFONT hFont = CreateFont(
        50,               // Height of the font (increase this to make pieces bigger)
        0,                // Width of the font (0 for default aspect ratio)
        0,                // Angle of escapement
        0,                // Orientation angle
        FW_NORMAL,        // Font weight (FW_BOLD for bold text)
        FALSE,            // Italic
        FALSE,            // Underline
        FALSE,            // Strikeout
        DEFAULT_CHARSET,  // Character set
        OUT_DEFAULT_PRECIS, // Output precision
        CLIP_DEFAULT_PRECIS, // Clipping precision
        DEFAULT_QUALITY,    // Output quality
        DEFAULT_PITCH | FF_SWISS, // Pitch and family
        L"Segoe UI Emoji"  // Font name (supports chess piece symbols)
    );

    // Select the custom font into the HDC
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    // Set background transparency and text alignment
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0)); // Black for simplicity

    // Define the cell's rectangle
    RECT rect = { col * CELL_SIZE, row * CELL_SIZE, (col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE };

    // Draw ASCII art in the center of the cell
    std::wstring art = pieceArt[piece];
    DrawTextW(hdc, art.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Restore the old font and delete the custom font
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

void PlaySoundEffect(LPCWSTR soundFile) {
    PlaySound(soundFile, NULL, SND_FILENAME | SND_ASYNC);
}

bool IsPlayerPiece(POINT position) {
    std::wstring piece = chessboard[position.y * BOARD_SIZE + position.x];
    if (piece.empty()) return false; // Add this line to handle empty squares
    return (playerTurn == (piece.back() == L'B')); // Check if the piece belongs to the current player
}


bool ValidMove(POINT from, POINT to) {
    if (chessboard[from.y * BOARD_SIZE + from.x].empty()) return false; // Prevent moving from an empty square

    if (!IsPlayerPiece(from)) {
        return false; // Only allow the player to move their own pieces
    }

    std::wstring piece = chessboard[from.y * BOARD_SIZE + from.x];
    wchar_t pieceType = piece[0];  // 'P' for pawn, 'R' for rook, etc.

    // Get the color of the piece
    bool isWhite = (piece.back() == L'W');
    std::wstring destinationPiece = chessboard[to.y * BOARD_SIZE + to.x];

    // Check if the move is capturing the king
    if (IsMoveCapturingKing(to)) return false; // Prevent capturing the king

    // Rook movement (Horizontal/Vertical)
    if (pieceType == L'R') {
        if (from.x != to.x && from.y != to.y) return false; // Rook can only move in a straight line
        // Check if the path is blocked
        int stepX = (to.x > from.x) ? 1 : (to.x < from.x) ? -1 : 0;
        int stepY = (to.y > from.y) ? 1 : (to.y < from.y) ? -1 : 0;
        for (int i = 1; i < max(abs(to.x - from.x), abs(to.y - from.y)); ++i) {
            if (!chessboard[(from.y + i * stepY) * BOARD_SIZE + (from.x + i * stepX)].empty()) {
                return false; // Path is blocked
            }
        }
    }

    // Knight movement (L-shaped)
    else if (pieceType == L'N') {
        if (!((abs(from.x - to.x) == 2 && abs(from.y - to.y) == 1) ||
            (abs(from.x - to.x) == 1 && abs(from.y - to.y) == 2))) {
            return false; // Knight moves in an L-shape
        }
    }

    // Bishop movement (Diagonal)
    else if (pieceType == L'B') {
        if (abs(from.x - to.x) != abs(from.y - to.y)) return false; // Bishop moves diagonally
        // Check if the path is blocked
        int stepX = (to.x > from.x) ? 1 : -1;
        int stepY = (to.y > from.y) ? 1 : -1;
        for (int i = 1; i < abs(to.x - from.x); ++i) {
            if (!chessboard[(from.y + i * stepY) * BOARD_SIZE + (from.x + i * stepX)].empty()) return false; // Path is blocked
        }
    }

    // Queen movement (Combination of Rook and Bishop)
    else if (pieceType == L'Q') {
        if ((from.x != to.x && from.y != to.y) &&
            abs(from.x - to.x) != abs(from.y - to.y)) return false; // Queen moves like a rook or bishop
        int stepX = (to.x > from.x) ? 1 : (to.x < from.x) ? -1 : 0;
        int stepY = (to.y > from.y) ? 1 : (to.y < from.y) ? -1 : 0;
        for (int i = 1; i < max(abs(to.x - from.x), abs(to.y - from.y)); ++i) {
            if (!chessboard[(from.y + i * stepY) * BOARD_SIZE + (from.x + i * stepX)].empty()) return false; // Path is blocked
        }
    }

    // King movement (One square in any direction)
    else if (pieceType == L'K') {
        if (abs(from.x - to.x) > 1 || abs(from.y - to.y) > 1) return false; // King moves one square in any direction
    }

    // Pawn movement (One square forward, two squares on the first move, capture diagonally)
    else if (pieceType == L'P') {
        if (isWhite) {
            if (from.y == 6 && to.y == from.y - 2 && from.x == to.x && destinationPiece.empty()) return true;
            if (to.y == from.y - 1 && from.x == to.x && destinationPiece.empty()) return true; // Regular move
            if (abs(from.x - to.x) == 1 && to.y == from.y - 1 && !destinationPiece.empty() && destinationPiece.back() == L'B') {
                return true; // Capture diagonally
            }
        }
        else {
            if (from.y == 1 && to.y == from.y + 2 && from.x == to.x && destinationPiece.empty()) return true;
            if (to.y == from.y + 1 && from.x == to.x && destinationPiece.empty()) return true; // Regular move
            if (abs(from.x - to.x) == 1 && to.y == from.y + 1 && !destinationPiece.empty() && destinationPiece.back() == L'W') {
                return true; // Capture diagonally
            }
        }
        return false; // All other moves are invalid for pawns
    }

    // Invalid piece type or move
    return destinationPiece.empty() || destinationPiece.back() != piece.back();
}

bool IsKingInCheck(POINT kingPos, bool isWhite) {
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            POINT opponentPos = { x, y };
            std::wstring piece = chessboard[y * BOARD_SIZE + x];

            if (piece.empty() || piece.back() == (isWhite ? L'W' : L'B')) continue;

            if (ValidMove(opponentPos, kingPos)) return true;
        }
    }
    return false;
}

void MovePiece(POINT from, POINT to) {
    if (ValidMove(from, to)) {
        std::wstring movingPiece = chessboard[from.y * BOARD_SIZE + from.x];
        std::wstring capturedPiece = chessboard[to.y * BOARD_SIZE + to.x];

        // Temporarily move the piece
        chessboard[to.y * BOARD_SIZE + to.x] = movingPiece;
        chessboard[from.y * BOARD_SIZE + from.x] = L"";

        // Update king positions if a king is moved
        if (movingPiece[0] == L'K') {
            if (movingPiece.back() == L'W') {
                whiteKingPos = to;
            }
            else {
                blackKingPos = to;
            }
        }

        // Check for king in check
        bool isKingInCheck = (movingPiece.back() == L'W')
            ? IsKingInCheck(whiteKingPos, true)
            : IsKingInCheck(blackKingPos, false);

        if (isKingInCheck) {
            // Revert the move if it places the king in check
            chessboard[from.y * BOARD_SIZE + from.x] = movingPiece;
            chessboard[to.y * BOARD_SIZE + to.x] = capturedPiece;
            std::wcout << L"Move places the king in check. Move invalid!" << std::endl;
            return;
        }

        // Play capture sound if capturing a piece
        if (!capturedPiece.empty()) {
            PlaySoundEffect(L"capture.wav");
        }
        else {
            PlaySoundEffect(L"move.wav");
        }

        TurnChange(playerTurn);
    }
}

void TurnChange(bool& turn) {
    turn = !turn;
    std::wcout << (turn ? L"Player 2's Turn" : L"Player 1's Turn") << std::endl;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawChessboard(hdc);
        EndPaint(hWnd, &ps);
    }
                 break;

    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam) / CELL_SIZE;
        int y = HIWORD(lParam) / CELL_SIZE;

        if (selectedPiece.x == -1 && selectedPiece.y == -1) {
            selectedPiece = { x, y };
            CalculateValidMoves(selectedPiece); // Calculate valid moves for the selected piece
        }
        else {
            MovePiece(selectedPiece, { x, y });
            selectedPiece = { -1, -1 };
            validMoves.clear(); // Clear valid moves after a move is made
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        InvalidateRect(hWnd, nullptr, TRUE); // Redraw the board to show the valid moves
    }
                       break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

