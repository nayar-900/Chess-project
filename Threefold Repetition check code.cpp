#include <unordered_map> 
#include <vector>   

unordered_map<string, int> board_state_count;

string generate_board_hash(LinkedList& board) {
    Node* current = board.getNode(0, 0);
    string hash = "";
    while (current) {
        hash += current->piece;
        current = current->next;
    }
    return hash;
}

void update_board_state(LinkedList& board) {
    string board_hash = generate_board_hash(board);
    if (board_state_count.find(board_hash) == board_state_count.end()) {
        board_state_count[board_hash] = 1;
    } else {
        board_state_count[board_hash]++;
    }
}

bool check_threefold_repetition() {
    for (auto& state : board_state_count) {
        if (state.second >= 3) {
            cout << "Threefold repetition detected! Game is a draw." << endl;
            return true;
        }
    }
    return false;
}

void come_on(LinkedList& board, bool turn)
{
    while (true)
    {

        update_board_state(board);

        if (check_threefold_repetition()) {
            cout << "Game over ... !" << endl;
            break;
        }

        if (check_winner(board))
        {
            cout << "Game over ... !" << endl;
            break;
        }

        int choice;
        cout << "\nEnter your choice:\n";
        cout << "1. Make a move\n";
        cout << "2. Undo last move\n";
        cout << "3. Redo last undone move\n";
        cout << "Choice: ";
        cin >> choice;

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
            cout << "It's not your turn or invalid piece." << endl;
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
            char captured_piece = to->piece;
            to->piece = from->piece;
            from->piece = ' ';

            undo_stack.push({from_r, from_c, to_r, to_c, to->piece, captured_piece});

            while (!redo_stack.empty()) {
                redo_stack.pop();
            }

            updateMovementFlags(from);

            turn_change(turn);
            printBoard();
        }
        else
        {
            cout << "Invalid move! Try again." << endl;
        }
    }
}
