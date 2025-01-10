//function
void promote_pawn(Node* pawn, bool turn) {
    char promotion_piece;
    cout << "Your pawn has reached the last rank!" << endl;
    cout << "Choose a piece for promotion (Q, R, B, N): ";
    cin >> promotion_piece;

    promotion_piece = toupper(promotion_piece);

    while (promotion_piece != 'Q' && promotion_piece != 'R' && promotion_piece != 'B' && promotion_piece != 'N') {
        cout << "Invalid choice! Choose one of the following: Q, R, B, N: ";
        cin >> promotion_piece;
        promotion_piece = toupper(promotion_piece);
    }

    if (!turn) 
    {
        promotion_piece = tolower(promotion_piece);
    }
    pawn->piece = promotion_piece;

    cout << "Pawn promoted to " << promotion_piece << "!" << endl;
}
// modified come-on function
void come_on(LinkedList& board, bool turn) {
    while (true) {
        int from_r, from_c, to_r, to_c;
        string command;

        cout << "Enter 'undo', 'redo', or make a move (row and column): ";
        cin >> command;

        if (command == "undo") {
            undo_move(board, turn);
            continue;
        } else if (command == "redo") {
            redo_move(board, turn);
            continue;
        } else {
            from_r = stoi(command);
            cin >> from_c >> to_r >> to_c;
        }

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
            Move move = {from_r, from_c, to_r, to_c, piece, to->piece};
            undo_stack.push(move);

            while (!redo_stack.empty()) {
                redo_stack.pop();
            }

            from->piece = ' ';
            to->piece = piece;

            if (tolower(piece) == 'p' && ((turn && to_r == 0) || (!turn && to_r == 7))) {
                promote_pawn(to, turn);
            }

            print_board(board);
        } else {
            cout << "Invalid move! Please enter correct positions." << endl;
            continue;
        }

        save_game(board, turn);
        turn_change(turn);
    }
}
