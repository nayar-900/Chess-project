#include <iostream>
#include <cctype>
#include <stack>
#include <unordered_map>
#include <vector>     
using namespace std;


unordered_map <string, int> board_state_count;

bool white_king_moved = false;
bool black_king_moved = false;
bool white_rook_left_moved = false;
bool white_rook_right_moved = false;
bool black_rook_left_moved = false;
bool black_rook_right_moved = false;

struct Node
{
    char piece;
    Node* next;
    Node* prev;
    int row, col;
    bool hasMoved;
};

struct Move
{
    int from_r, from_c, to_r, to_c;
    char moved_piece;
    char captured_piece;
};

void turn_change(bool& turn)
{
    turn = !turn;
    cout << endl;
    cout << "Player " << (turn ? 2 : 1) << " turn" << endl;
}

stack<Move> undo_stack;
stack<Move> redo_stack;

class LinkedList
{
public:
    LinkedList(int size)
    {
        head = nullptr;
        tail = nullptr;
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                Node* newNode = new Node();
                newNode->piece = ' ';
                newNode->row = i;
                newNode->col = j;
                newNode->hasMoved = false;
                newNode->next = nullptr;
                newNode->prev = tail;
                if (tail)
                {
                    tail->next = newNode;
                }
                tail = newNode;
                if (!head)
                {
                    head = newNode;
                }
            }
        }
    }

    Node* getNode(int row, int col)
    {
        Node* current = head;
        while (current)
        {
            if (current->row == row && current->col == col)
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void printBoard()
    {
        Node* current = head;
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                cout << "" << current->piece << "|";
                current = current->next;
            }
            cout << "  " << 8 - i << endl;
        }
        cout << endl;
        for (int i=1; i<=8; i++)
        {
            cout << " " << i << "  ";
        }
        cout << endl;
    }

    void undo_move(LinkedList& board, bool& turn)
    {
        if (undo_stack.empty())
        {
            cout << "No moves to undo ... !" << endl;
            return;
        }

        Move last_move = undo_stack.top();
        undo_stack.pop();

        Node* from = board.getNode(last_move.from_r, last_move.from_c);
        Node* to = board.getNode(last_move.to_r, last_move.to_c);

        from->piece = last_move.moved_piece;
        to->piece = last_move.captured_piece;

        redo_stack.push(last_move);
        cout << endl;
        cout << "Move undone ... !" << endl;
        turn_change(turn);
        cout << endl;
        printBoard();
    }

    void redo_move(LinkedList& board, bool& turn)
    {
        if (redo_stack.empty())
        {
            cout << "No moves to redo!" << endl;
            return;
        }

        Move last_move = redo_stack.top();
        redo_stack.pop();

        Node* from = board.getNode(last_move.from_r, last_move.from_c);
        Node* to = board.getNode(last_move.to_r, last_move.to_c);

        from->piece = ' ';
        to->piece = last_move.moved_piece;

        undo_stack.push(last_move);
        cout << endl;
        cout << "Move undone ... !" << endl;
        turn_change(turn);
        cout << endl;
        printBoard();
    }

private:
    Node* head;
    Node* tail;
};

bool valid_move(char piece, bool turn)
{
    return turn ? isupper(piece) : islower(piece);
}

void init(LinkedList& board)
{
    string p1 = "RNBKQBNR";
    string p2 = "rnbkqbnr";
    for (int i = 0; i < 8; ++i)
    {
        board.getNode(0, i)->piece = p1[i];
        board.getNode(1, i)->piece = 'P';
        board.getNode(6, i)->piece = 'p';
        board.getNode(7, i)->piece = p2[i];
    }
}

bool pawn_move(char piece, Node* from, Node* to)
{
    if (piece == 'P')
    {
        if (!from->hasMoved && (to->row - from->row == 2) && (from->col == to->col) && to->piece == ' ')
        {
            from->hasMoved = true;
            return true;
        }
        if ((to->row - from->row == 1) && (from->col == to->col) && to->piece == ' ')
        {
            from->hasMoved = true;
            return true;
        }
        if ((to->row - from->row == 1) && abs(to->col - from->col) == 1 && to->piece != ' ' && islower(to->piece))
        {
            from->hasMoved = true;
            return true;
        }
    }
    else if (piece == 'p')
    {
        if (!from->hasMoved && (from->row - to->row == 2) && (from->col == to->col) && to->piece == ' ')
        {
            from->hasMoved = true;
            return true;
        }
        if ((from->row - to->row == 1) && (from->col == to->col) && to->piece == ' ')
        {
            from->hasMoved = true;
            return true;
        }
        if ((from->row - to->row == 1) && abs(to->col - from->col) == 1 && to->piece != ' ' && isupper(to->piece))
        {
            from->hasMoved = true;
            return true;
        }
    }
    return false;
}

void updateMovementFlags(Node* from)
{
    char piece = from->piece;

    if (piece == 'K')
    {
        white_king_moved = true;
    }
    else if (piece == 'k')
    {
        black_king_moved = true;
    }
    else if (piece == 'R' && from->row == 7 && from->col == 0)
    {
        white_rook_left_moved = true;
    }
    else if (piece == 'R' && from->row == 7 && from->col == 7)
    {
        white_rook_right_moved = true;
    }
    else if (piece == 'r' && from->row == 0 && from->col == 0)
    {
        black_rook_left_moved = true;
    }
    else if (piece == 'r' && from->row == 0 && from->col == 7)
    {
        black_rook_right_moved = true;
    }
}

void executeKingSideCastling(LinkedList& board, int row, bool isBlack)
{
    Node* king = board.getNode(row, 4);
    Node* rook = board.getNode(row, 7);
    Node* newKing = board.getNode(row, 6);
    Node* newRook = board.getNode(row, 5);

    newKing->piece = king->piece;
    king->piece = ' ';
    newRook->piece = rook->piece;
    rook->piece = ' ';
}

void executeQueenSideCastling(LinkedList& board, int row, bool isBlack)
{
    Node* king = board.getNode(row, 4);
    Node* rook = board.getNode(row, 0);
    Node* newKing = board.getNode(row, 2);
    Node* newRook = board.getNode(row, 3);

    newKing->piece = king->piece;
    king->piece = ' ';
    newRook->piece = rook->piece;
    rook->piece = ' ';
}

bool canCastleKingSide(LinkedList& board, int row)
{
    Node* f1 = board.getNode(row, 5);
    Node* f2 = board.getNode(row, 6);
    return f1->piece == ' ' && f2->piece == ' ';
}

bool canCastleQueenSide(LinkedList& board, int row)
{
    Node* f1 = board.getNode(row, 3);
    Node* f2 = board.getNode(row, 2);
    Node* f3 = board.getNode(row, 1);
    return f1->piece == ' ' && f2->piece == ' ' && f3->piece == ' ';
}

bool is_opponent_piece(char piece, char target_piece)
{
    return (isupper(piece) && islower(target_piece)) || (islower(piece) && isupper(target_piece));
}

bool knight_move(Node* from, Node* to)
{
    int row_diff = abs(to->row - from->row);
    int col_diff = abs(to->col - from->col);

    return (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
}

bool rook_move(Node* from, Node* to, LinkedList& board)
{
    if (from->row == to->row)
    {

        int min_col = min(from->col, to->col);
        int max_col = max(from->col, to->col);
        for (int col = min_col + 1; col < max_col; ++col)
        {
            if (board.getNode(from->row, col)->piece != ' ')
            {
                return false;
            }
        }
        return true;
    }
    else if (from->col == to->col)
    {

        int min_row = min(from->row, to->row);
        int max_row = max(from->row, to->row);
        for (int row = min_row + 1; row < max_row; ++row)
        {
            if (board.getNode(row, from->col)->piece != ' ')
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool bishop_move(Node* from, Node* to, LinkedList& board)
{
    int row_diff = abs(to->row - from->row);
    int col_diff = abs(to->col - from->col);

    if (row_diff == col_diff)
    {
        int row_dir = (to->row > from->row) ? 1 : -1;
        int col_dir = (to->col > from->col) ? 1 : -1;

        for (int i = 1; i < row_diff; ++i)
        {
            int row = from->row + i * row_dir;
            int col = from->col + i * col_dir;
            if (board.getNode(row, col)->piece != ' ')
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool queen_move(Node* from, Node* to, LinkedList& board)
{

    return rook_move(from, to, board) || bishop_move(from, to, board);
}

bool king_move(Node* from, Node* to, LinkedList& board, bool turn)
{
    int row_diff = abs(from->row - to->row);
    int col_diff = abs(from->col - to->col);

    if (row_diff <= 1 && col_diff <= 1)
    {
        return true;
    }

    if (!turn && from->piece == 'K' && !white_king_moved)
    {
        if (to->row == from->row && (to->col == from->col + 2 || to->col == from->col - 2))
        {

            if (to->col == from->col + 2 && !white_rook_right_moved)
            {
                if (canCastleKingSide(board, from->row))
                {
                    executeKingSideCastling(board, from->row, false);
                    white_king_moved = true;
                    white_rook_right_moved = true;
                    return true;
                }
            }

            if (to->col == from->col - 2 && !white_rook_left_moved)
            {
                if (canCastleQueenSide(board, from->row))
                {
                    executeQueenSideCastling(board, from->row, false);
                    white_king_moved = true;
                    white_rook_left_moved = true;
                    return true;
                }
            }
        }
    }
    else if (turn && from->piece == 'k' && !black_king_moved) {
        if (to->row == from->row && (to->col == from->col + 2 || to->col == from->col - 2))
        {

            if (to->col == from->col + 2 && !black_rook_right_moved)
            {
                if (canCastleKingSide(board, from->row))
                {
                    executeKingSideCastling(board, from->row, true);
                    black_king_moved = true;
                    black_rook_right_moved = true;
                    return true;
                }
            }

            if (to->col == from->col - 2 && !black_rook_left_moved)
            {
                if (canCastleQueenSide(board, from->row))
                {
                    executeQueenSideCastling(board, from->row, true);
                    black_king_moved = true;
                    black_rook_left_moved = true;
                    return true;
                }
            }
        }
    }

    return false;
}

void pawn_promotion(Node* to, bool turn)
{
    if ((turn && to->row == 7 && to->piece == 'P') || (!turn && to->row == 0 && to->piece == 'p'))
    {
        char choice;
        cout << "Pawn reached the end of the board! Choose a piece for promotion: \n";
        cout << "Q: Queen\nR: Rook\nB: Bishop\nN: Knight" << endl;

        while (true)
        {
            cout << "Enter your choice: ";
            cin >> choice;
            choice = toupper(choice);
            if (choice == 'Q' || choice == 'R' || choice == 'B' || choice == 'N')
            {
                to->piece = (turn ? choice : tolower(choice));
                cout << "Pawn promoted to " << (turn ? choice : tolower(choice)) << "!" << endl;
                break;
            }
            else
            {
                cout << "Invalid choice. Please select again." << endl;
            }
        }
    }
}

bool is_king_present(LinkedList& board, char king)
{
    Node* current = board.getNode(0, 0);
    while (current)
    {
        if (current->piece == king)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool check_winner(LinkedList& board)
{
    bool white_king_present = is_king_present(board, 'K');
    bool black_king_present = is_king_present(board, 'k');

    if (!white_king_present)
    {
        cout << "\nBlack wins! White's king is captured." << endl;
        return true;
    }
    if (!black_king_present)
    {
        cout << "\nWhite wins! Black's king is captured." << endl;
        return true;
    }

    return false;
}

string generate_board_hash(LinkedList& board)
{
    Node* current = board.getNode(0, 0);
    string hash = "";
    while (current)
    {
        hash += current->piece;
        current = current->next;
    }
    return hash;
}

void update_board_state(LinkedList& board)
{
    string board_hash = generate_board_hash(board);
    if (board_state_count.find(board_hash) == board_state_count.end())
    {
        board_state_count[board_hash] = 1;
    }
    else
    {
        board_state_count[board_hash]++;
    }
}

/*bool check_threefold_repetition()
{
    for (auto& state : board_state_count)
    {
        if (state.second >= 3)
        {
            cout << "Threefold repetition detected ... ! Game is a draw ..." << endl;
            return true;
        }
    }
    return false;
}*/

void come_on(LinkedList& board, bool turn)
{
    while (true)
    {
        update_board_state(board);

        /*if (check_threefold_repetition())
        {
            cout << "Game over ... !" << endl;
            break;
        }*/

        if (check_winner(board))
        {
            cout << "Game over ... !" << endl;
            break;
        }

        int choice;
        cout << endl;
        cout << "Enter your choice:" << endl;
        cout << "1. Make a move" << endl;
        cout << "2. Undo last move" << endl;
        cout << "3. Redo last undone move" << endl;
        cout << "Choice: ";
        cin >> choice;
        cout << endl;
        if (choice == 2)
        {
            board.undo_move(board, turn);
            continue;
        }
        else if (choice == 3)
        {
            board.redo_move(board, turn);
            continue;
        }
        else if (choice != 1)
        {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

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

        if (!valid_move(piece, turn))
        {
            cout << "It's not your turn or invalid piece ... " << endl;
            continue;
        }

        if (piece == ' ' || (to->piece != ' ' && !is_opponent_piece(piece, to->piece)))
        {
            cout << "Invalid move! You can't capture your own piece." << endl;
            continue;
        }

        bool valid = false;
        switch (tolower(piece))
        {
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
                valid = king_move(from, to, board, turn);
                break;
            default:
                cout << "Invalid piece!" << endl;
                break;
        }

        if (valid)
        {

            Move move;
            move.from_r = from_r;
            move.from_c = from_c;
            move.to_r = to_r;
            move.to_c = to_c;
            move.moved_piece = piece;
            move.captured_piece = to->piece;

            undo_stack.push(move);

            while (!redo_stack.empty())
            {
                redo_stack.pop();
            }
            from->piece = ' ';
            to->piece = piece;
            board.printBoard();
            pawn_promotion(to, turn);
            char undo_choice;
            cout << endl;
            cout << "Do you want to undo your move ... ? (y/n): ";
            cin >> undo_choice;
            if (tolower(undo_choice) == 'y')
            {
                board.undo_move(board, turn);
                continue;
            }
        }
        else
        {
            cout << "Invalid move ... ! Please enter correct positions ..." << endl;
            continue;
        }
        turn_change(turn);
    }
}

int main()
{
    LinkedList board(8);
    cout << endl << "Welcome to chess ... !" << endl << endl;
    cout << "Player 1 = small pieces" << endl;
    cout << "Player 2 = large pieces" << endl << endl;
    init(board);
    board.printBoard();
    bool turn = 0;
    cout << endl << "Player 1 turn" << endl;
    come_on(board, turn);
    return 0;
}
