//function
bool is_path_clear_castling(Node* king, Node* rook, LinkedList& board) {
    int row = king->row;
    int min_col = min(king->col, rook->col);
    int max_col = max(king->col, rook->col);

    for (int col = min_col + 1; col < max_col; ++col) {
        if (board.getNode(row, col)->piece != ' ') {
            return false;
        }
    }
    return true;
}

bool is_castling_valid(Node* king, Node* rook, LinkedList& board, bool turn) {
    if (king->hasMoved || rook->hasMoved) {
        return false;
    }

    if (!is_path_clear_castling(king, rook, board)) {
        return false;
    }
    int direction = (rook->col > king->col) ? 1 : -1;
    for (int i = 1; i <= 2; ++i) {
        Node* intermediate_square = board.getNode(king->row, king->col + i * direction);
        if (intermediate_square->piece != ' ') {
            return false;
        }

    }

    return true;
}

bool castling_move(Node* king, Node* rook, LinkedList& board) {
    if (is_castling_valid(king, rook, board, isupper(king->piece))) {
        int king_new_col = (rook->col > king->col) ? king->col + 2 : king->col - 2;
        int rook_new_col = (rook->col > king->col) ? king->col + 1 : king->col - 1;

        // Update the board
        Node* king_new_pos = board.getNode(king->row, king_new_col);
        Node* rook_new_pos = board.getNode(rook->row, rook_new_col);

        king_new_pos->piece = king->piece;
        rook_new_pos->piece = rook->piece;

        king->piece = ' ';
        rook->piece = ' ';

        king_new_pos->hasMoved = true;
        rook_new_pos->hasMoved = true;

        return true;
    }
    return false;
}
// modified come-on function
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

        if (tolower(piece) == 'k' && abs(to->col - from->col) == 2) {
            Node* rook = (to->col > from->col) ? board.getNode(from_r, 7) : board.getNode(from_r, 0);
            valid = castling_move(from, rook, board);
            if (!valid) {
                cout << "Invalid castling move!" << endl;
                continue;
            }
        } else {
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
        }

        if (valid) {
            from->piece = ' ';
            to->piece = piece;
            board.printBoard();
        } else {
            cout << "Invalid move! Please enter correct positions." << endl;
            continue;
        }

        turn_change(turn);
    }
}
