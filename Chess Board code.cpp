// Function to print the chessboard
void print_board(LinkedList& board) {
    cout << "\n  ";
    for (int i = 1; i <= 8; ++i) {
        cout << "  " << i << " ";
    }
    cout << endl;

    for (int i = 0; i < 8; ++i) {
        cout << "  +";
        for (int j = 0; j < 8; ++j) {
            cout << "---+";
        }
        cout << endl;

        cout << 8 - i << " |";
        for (int j = 0; j < 8; ++j) {
            Node* temp = board.getNode(i, j);
            cout << " " << temp->piece << " |";
        }
        cout << " " << 8 - i << endl;
    }
    cout << "  +";
    for (int j = 0; j < 8; ++j) {
        cout << "---+";
    }
    cout << endl;

    cout << "  ";
    for (int i = 1; i <= 8; ++i) {
        cout << "  " << i << " ";
    }
    cout << "\n" << endl;
}
//come-on function for chess board printing ... it is necessary for printing it
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

            // Call the separate board printing function
            print_board(board);
        } else {
            cout << "Invalid move! Please enter correct positions." << endl;
            continue;
        }

        save_game(board, turn);
        turn_change(turn);
    }
}
