#include <iostream>
#include <string>
#include <memory>
using namespace std;
struct ChessPiece {
    string type;
    string color; 
    ChessPiece(string t, string c) : type(t), color(c) {}
};
struct Node {
    ChessPiece* piece; 
    Node* next;     
    Node(ChessPiece* p) : piece(p), next(nullptr) {}
};
class ChessBoard {
private:
    Node* head; 
public:
    ChessBoard() : head(nullptr) {
        initializeBoard();
    }
    void initializeBoard() {
        string pieces[] = {"Rook", "Knight", "Bishop", "Queen", "King", "Bishop", "Knight", "Rook"};
        Node* current = nullptr;
        for (int i = 0; i < 8; ++i) {
            if (i == 0 || i == 7) {
                current = new Node(new ChessPiece(pieces[i], "White"));
            } else if (i == 1 || i == 6) { 
                current = new Node(new ChessPiece(pieces[i], "White"));
            } else if (i == 2 || i == 5) { 
                current = new Node(new ChessPiece(pieces[i], "White"));
            } else if (i == 3) { 
                current = new Node(new ChessPiece("Queen", "White"));
            } else { 
                current = new Node(new ChessPiece("King", "White"));
            }
            if (i != 7) {
                current->next = new Node(nullptr);
            }
        }
    }
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
int main() {
    ChessBoard board;
    board.printBoard();
    return 0;
}
