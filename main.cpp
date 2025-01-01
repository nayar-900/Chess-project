#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Define a structure for a Chess Piece
struct ChessPiece {
    string type; // "King", "Queen", "Pawn", etc.
    string color; // "White" or "Black"

    ChessPiece(string t, string c) : type(t), color(c) {}
};

// Define a node for the Linked List that represents each square on the chessboard
struct Node {
    ChessPiece* piece;  // Chess piece at that square
    Node* next;         // Pointer to the next square (node)

    Node(ChessPiece* p) : piece(p), next(nullptr) {}
};

// Class to represent a Chessboard using Linked List
class ChessBoard {
private:
    Node* head;  // Head of the linked list

public:
    ChessBoard() : head(nullptr) {
        initializeBoard();
    }

    // Function to initialize the chessboard with pieces
    void initializeBoard() {
        // Representing each row as a linked list
        string pieces[] = {"Rook", "Knight", "Bishop", "Queen", "King", "Bishop", "Knight", "Rook"};

        // Set up the black and white pieces on the board
        Node* current = nullptr;

        // Initialize white pieces
        for (int i = 0; i < 8; ++i) {
            if (i == 0 || i == 7) {  // Rooks
                current = new Node(new ChessPiece(pieces[i], "White"));
            } else if (i == 1 || i == 6) {  // Knights
                current = new Node(new ChessPiece(pieces[i], "White"));
            } else if (i == 2 || i == 5) {  // Bishops
                current = new Node(new ChessPiece(pieces[i], "White"));
            } else if (i == 3) {  // Queen
                current = new Node(new ChessPiece("Queen", "White"));
            } else {  // King
                current = new Node(new ChessPiece("King", "White"));
            }

            // Link the current node to the next node (for a row)
            // Set next as nullptr for the last square in the row
            if (i != 7) {
                current->next = new Node(nullptr);
            }
        }
    }

    // Function to print the chessboard
    void printBoard() {
        Node* current = head;
        for (int i = 0; i < 8; ++i) {
            cout << "| ";
            for (int j = 0; j < 8; ++j) {
                if (current->piece != nullptr) {
                    cout << current->piece->color[0] << current->piece->type[0] << " | ";
                } else {
                    cout << "   | ";
                }
                current = current->next;
            }
            cout << endl;
            current = head;
        }
    }
};

// Main Function to test the chessboard representation
int main() {
    ChessBoard board;
    board.printBoard();
    return 0;
}
