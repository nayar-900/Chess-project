//function
bool is_game_over(LinkedList& board, bool turn) {
    if (is_king_in_check(board, turn)) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Node* from = board.getNode(i, j);
                char piece = from->piece;

                if (piece == ' ' || (turn && islower(piece)) || (!turn && isupper(piece))) {
                    continue;
                }

                for (int x = 0; x < 8; ++x) {
                    for (int y = 0; y < 8; ++y) {
                        Node* to = board.getNode(x, y);

                        char original_to_piece = to->piece;
                        to->piece = piece;
                        from->piece = ' ';
                        bool still_in_check = is_king_in_check(board, turn);

                        from->piece = piece;
                        to->piece = original_to_piece;

                        if (!still_in_check) {
                            return false; 
                        }
                    }
                }
            }
        }

        cout << (turn ? "Black" : "White") << " wins by checkmate!" << endl;
        return true;
    } else {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Node* from = board.getNode(i, j);
                char piece = from->piece;

                if (piece == ' ' || (turn && islower(piece)) || (!turn && isupper(piece))) {
                    continue;
                }

                for (int x = 0; x < 8; ++x) {
                    for (int y = 0; y < 8; ++y) {
                        Node* to = board.getNode(x, y);
                        char original_to_piece = to->piece;

                        to->piece = piece;
                        from->piece = ' ';
                        bool still_in_check = is_king_in_check(board, turn);

                        from->piece = piece;
                        to->piece = original_to_piece;

                        if (!still_in_check) {
                            return false;
                        }
                    }
                }
            }
        }
        cout << "Game ends in a stalemate!" << endl;
        return true;
    }
}
// modified come-on function code
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
            cout << "Invalid move!" << endl;
            continue;
        }

        to->piece = from->piece;
        from->piece = ' ';

        print_board(board);
      
        if (is_game_over(board, turn)) {
            break;
        }

        save_game(board, turn);
        turn_change(turn);
    }
}
