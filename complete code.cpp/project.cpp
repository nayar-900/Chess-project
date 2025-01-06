#include <iostream>
#include <cctype>
using namespace std;

struct Node {
    char piece;
    Node* next;
    Node* prev;
    int row, col;
    bool hasMoved;
};

class LinkedList {
public:
    LinkedList(int size) {
        head = nullptr;
        tail = nullptr;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                Node* newNode = new Node();
                newNode->piece = ' ';
                newNode->row = i;
                newNode->col = j;
                newNode->hasMoved = false;
                newNode->next = nullptr;
                newNode->prev = tail;
                if (tail) {
                    tail->next = newNode;
                }
                tail = newNode;
                if (!head) {
                    head = newNode;
                }
            }
        }
    }

    Node* getNode(int row, int col) {
        Node* current = head;
        while (current) {
            if (current->row == row && current->col == col) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void printBoard() {
        Node* current = head;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                cout << "_" << current->piece << "_|";
                current = current->next;
            }
            cout << "  " << 8 - i << endl;
        }
        cout << endl;
        for (int i=1; i<=8; i++) {
            cout << " " << i << "  ";
        }
        cout << endl;
    }

private:
    Node* head;
    Node* tail;
};

void turn_change(bool& turn) {
    turn = !turn;
    cout << endl;
    cout << "Player " << (turn ? 2 : 1) << " turn" << endl;
}

bool valid_move(char piece, bool turn) {
    return turn ? isupper(piece) : islower(piece);
}

void init(LinkedList& board) {
    string p1 = "RNBKQBNR";
    string p2 = "rnbkqbnr";
    for (int i = 0; i < 8; ++i) {
        board.getNode(0, i)->piece = p1[i];
        board.getNode(1, i)->piece = 'P';
        board.getNode(6, i)->piece = 'p';
        board.getNode(7, i)->piece = p2[i];
    }
}

bool pawn_move(char piece, Node* from, Node* to) {
    if (piece == 'P') {
        if (!from->hasMoved && (to->row - from->row == 2) && (from->col == to->col) && to->piece == ' ') {
            from->hasMoved = true;
            return true;
        }
        if ((to->row - from->row == 1) && (from->col == to->col) && to->piece == ' ') {
            from->hasMoved = true;
            return true;
        }
        if ((to->row - from->row == 1) && abs(to->col - from->col) == 1 && to->piece != ' ' && islower(to->piece)) {
            from->hasMoved = true;
            return true;
        }
    } else if (piece == 'p') {
        if (!from->hasMoved && (from->row - to->row == 2) && (from->col == to->col) && to->piece == ' ') {
            from->hasMoved = true;
            return true;
        }
        if ((from->row - to->row == 1) && (from->col == to->col) && to->piece == ' ') {
            from->hasMoved = true;
            return true;
        }
        if ((from->row - to->row == 1) && abs(to->col - from->col) == 1 && to->piece != ' ' && isupper(to->piece)) {
            from->hasMoved = true;
            return true;
        }
    }
    return false;
}

bool knight_move(Node* from, Node* to) {
    int row_diff = abs(to->row - from->row);
    int col_diff = abs(to->col - from->col);

    // Knight moves in "L" shape: 2 squares in one direction and 1 in the other
    return (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
}

bool rook_move(Node* from, Node* to, LinkedList& board) {
    if (from->row == to->row) {
        // Horizontal movement
        int min_col = min(from->col, to->col);
        int max_col = max(from->col, to->col);
        for (int col = min_col + 1; col < max_col; ++col) {
            if (board.getNode(from->row, col)->piece != ' ') {
                return false;
            }
        }
        return true;
    } else if (from->col == to->col) {
        // Vertical movement
        int min_row = min(from->row, to->row);
        int max_row = max(from->row, to->row);
        for (int row = min_row + 1; row < max_row; ++row) {
            if (board.getNode(row, from->col)->piece != ' ') {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool bishop_move(Node* from, Node* to, LinkedList& board) {
    int row_diff = abs(to->row - from->row);
    int col_diff = abs(to->col - from->col);

    if (row_diff == col_diff) { // Must move diagonally
        int row_dir = (to->row > from->row) ? 1 : -1;
        int col_dir = (to->col > from->col) ? 1 : -1;

        for (int i = 1; i < row_diff; ++i) {
            int row = from->row + i * row_dir;
            int col = from->col + i * col_dir;
            if (board.getNode(row, col)->piece != ' ') {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool is_opponent_piece(char piece, char target_piece) {
    return (isupper(piece) && islower(target_piece)) || (islower(piece) && isupper(target_piece));
}

bool queen_move(Node* from, Node* to, LinkedList& board) {
    return rook_move(from, to, board) || bishop_move(from, to, board);
}

bool king_move(Node* from, Node* to) {
    int row_diff = abs(to->row - from->row);
    int col_diff = abs(to->col - from->col);
    return row_diff <= 1 && col_diff <= 1;
}

void pawn_promotion(Node* to, bool turn) {
    if ((turn && to->row == 7 && to->piece == 'P') || (!turn && to->row == 0 && to->piece == 'p')) {
        char choice;
        cout << "Pawn reached the end of the board! Choose a piece for promotion: \n";
        cout << "Q: Queen\nR: Rook\nB: Bishop\nN: Knight" << endl;

        while (true) {
            cout << "Enter your choice: ";
            cin >> choice;
            choice = toupper(choice);
            if (choice == 'Q' || choice == 'R' || choice == 'B' || choice == 'N') {
                to->piece = (turn ? choice : tolower(choice));
                cout << "Pawn promoted to " << (turn ? choice : tolower(choice)) << "!" << endl;
                break;
            } else {
                cout << "Invalid choice. Please select again." << endl;
            }
        }
    }
}

void come_on(LinkedList& board, bool turn) {
    while (true) {
        int from_r, from_c, to_r, to_c;
        cout << "Enter the row and column to move from: ";
        cin >> from_r >> from_c;
        cout << "Enter the row and column to move to: ";
        cin >> to_r >> to_c;
        cout << endl;

        from_r = 8 - from_r;
        from_c -= 1;
        to_r = 8 - to_r;
        to_c -= 1;

        Node* from = board.getNode(from_r, from_c);
        Node* to = board.getNode(to_r, to_c);
        char piece = from->piece;

        if (!valid_move(piece, turn)) {
            cout << "It's not your turn or invalid piece." << endl;
            continue;
        }

        if (piece == ' ' || (to->piece != ' ' && !is_opponent_piece(piece, to->piece))) {
            cout << "Invalid move! You can't capture your own piece." << endl;
            continue;
        }

        bool valid = false;
        switch (tolower(piece)) {
            case 'p':
                valid = pawn_move(piece, from, to);
                break;
            case 'n':
                valid = knight_move(from, to);
                break;
            case 'r':
                valid = rook_move(from, to, board);
                break;
            case 'b':
                valid = bishop_move(from, to, board);
                break;
            case 'q':
                valid = queen_move(from, to, board);
                break;
            case 'k':
                valid = king_move(from, to);
                break;
            default:
                cout << "Invalid piece!" << endl;
                break;
        }

        if (valid) {
            from->piece = ' ';
            to->piece = piece;
            board.printBoard();

            pawn_promotion(to, turn);
        } else {
            cout << "Invalid move! Please enter correct positions." << endl;
            continue;
        }

        turn_change(turn); 
    }
}

int main() {
    LinkedList board(8);
    cout << endl << "Welcome to play the most amazing game of chess!" << endl << endl;
    cout << "Player 1 = small pieces" << endl;
    cout << "Player 2 = large pieces" << endl << endl;
    init(board);
    board.printBoard();
    bool turn = 0;
    cout << endl << "Player 1 turn" << endl;
    come_on(board, turn);
    return 0;
}
