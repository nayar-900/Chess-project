#include <fstream>

void save_game(LinkedList& board, bool turn, const string& filename) 
{
    ofstream file(filename);
    if (!file.is_open()) 
    {
        cout << "Error opening file for saving!" << endl;
        return;
    }

    Node* current = board.getNode(0, 0);
    while (current) 
    {
        file << current->piece << " ";
        current = current->next;
    }
    file << endl;

    file << turn << endl;

    file << white_king_moved << " " << black_king_moved << " "
         << white_rook_left_moved << " " << white_rook_right_moved << " "
         << black_rook_left_moved << " " << black_rook_right_moved << endl;

    cout << "Game saved successfully!" << endl;
    file.close();
}

void load_game(LinkedList& board, bool& turn, const string& filename) 
{
    ifstream file(filename);
    if (!file.is_open()) 
    {
        cout << "Error opening file for loading!" << endl;
        return;
    }

    Node* current = board.getNode(0, 0);
    while (current) 
    {
        file >> current->piece;
        current = current->next;
    }

    file >> turn;

    file >> white_king_moved >> black_king_moved
         >> white_rook_left_moved >> white_rook_right_moved
         >> black_rook_left_moved >> black_rook_right_moved;

    cout << "Game loaded successfully!" << endl;
    file.close();
}

void come_on(LinkedList& board, bool turn) 
{
    while (true) 
    {
        update_board_state(board);

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
        cout << "4. Save game" << endl;
        cout << "5. Load game" << endl;
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
        else if (choice == 4)
        {
            string filename;
            cout << "Enter filename to save the game: ";
            cin >> filename;
            save_game(board, turn, filename);
            continue;
        } 
        else if (choice == 5) 
        {
            string filename;
            cout << "Enter filename to load the game: ";
            cin >> filename;
            load_game(board, turn, filename);
            board.printBoard();
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

        if (!from || !to) 
        {
            cout << "Invalid coordinates!" << endl;
            continue;
        }

        if (!valid_move(from->piece, turn))
        {
            cout << "It's not your turn!" << endl;
            continue;
        }

        bool valid = false;
        switch (tolower(from->piece)) 
        {
            case 'p': valid = pawn_move(from->piece, from, to); break;
            case 'n': valid = knight_move(from, to); break;
            case 'r': valid = rook_move(from, to, board); break;
            case 'b': valid = bishop_move(from, to, board); break;
            case 'q': valid = queen_move(from, to, board); break;
            case 'k': valid = king_move(from, to, board, turn); break;
        }

        if (!valid || is_opponent_piece(from->piece, to->piece)) 
        {
            cout << "Invalid move!" << endl;
            continue;
        }

        undo_stack.push({from_r, from_c, to_r, to_c, from->piece, to->piece});
        redo_stack = stack<Move>();

        updateMovementFlags(from);
        to->piece = from->piece;
        from->piece = ' ';

        pawn_promotion(to, turn);

        turn_change(turn);
        board.printBoard();
    }
}
