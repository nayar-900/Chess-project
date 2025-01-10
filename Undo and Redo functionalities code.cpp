#include <stack>

struct Move {
    int from_r, from_c, to_r, to_c;
    char moved_piece;    
    char captured_piece; 
};

stack<Move> undo_stack;
stack<Move> redo_stack;
//undo and redo functions
void undo_move(LinkedList& board, bool& turn) {
    if (undo_stack.empty()) {
        cout << "No moves to undo!" << endl;
        return;
    }

    Move last_move = undo_stack.top();
    undo_stack.pop();

    Node* from = board.getNode(last_move.from_r, last_move.from_c);
    Node* to = board.getNode(last_move.to_r, last_move.to_c);

    from->piece = last_move.moved_piece;
    to->piece = last_move.captured_piece;

    redo_stack.push(last_move);

    turn_change(turn);

    cout << "Move undone!" << endl;
    print_board(board);
}

void redo_move(LinkedList& board, bool& turn) {
    if (redo_stack.empty()) {
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

    turn_change(turn);

    cout << "Move redone!" << endl;
    print_board(board);
}
// modified come-on function
void come_on(LinkedList& board, bool turn) {
    while (true) {
        int choice;
        cout << "\nEnter your choice:\n";
        cout << "1. Make a move\n";
        cout << "2. Undo last move\n";
        cout << "3. Redo last undone move\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 2) { // Undo
            board.undo_move(board, turn);
            continue;
        } else if (choice == 3) { // Redo
            board.redo_move(board, turn);
            continue;
        } else if (choice != 1) {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

        // Input move coordinates
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
            // Record move
            Move move;
            move.from_r = from_r;
            move.from_c = from_c;
            move.to_r = to_r;
            move.to_c = to_c;
            move.moved_piece = piece;
            move.captured_piece = to->piece;

            undo_stack.push(move);
            while (!redo_stack.empty()) {
                redo_stack.pop();
            }

            // Execute move
            from->piece = ' ';
            to->piece = piece;
            board.printBoard();

            pawn_promotion(to, turn);

            
            char undo_choice;
            cout << "Do you want to undo your move? (y/n): ";
            cin >> undo_choice;
            if (tolower(undo_choice) == 'y') {
                board.undo_move(board, turn);
                continue;  // Skip turn change since move is undone
            }

        } else {
            cout << "Invalid move ... ! Please enter correct positions ..." << endl;
            continue;
        }

        turn_change(turn);
    }
}
