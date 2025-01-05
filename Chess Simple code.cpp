#include <iostream>
using namespace std;
void turn_change (bool &turn)
{
if (turn == 0)
{
turn =1;
cout << "player 2 turn" << endl;
}
else
{
turn = 0;
cout << "player 1 turn" << endl;
}
}
void init( char A[], int s )
{
    int i;
    for (i=0; i<(s*s); i++)
    {
        A[i] = ' ';
    }
    A[0] = 'R';A[1] = 'N';A[2] = 'B';A[3] = 'K';A[4] = 'Q';A[5] = 'B';A[6] = 'N';A[7] = 'R';
    for(int i=8; i<=15;i++)
    {
    A[i] = 'P';
    }
    for(int i=48; i<=55;i++)
    {
    A[i] = 'p';
    }
    A[56] = 'r';A[57] = 'n';A[58] = 'b';A[59] = 'k';A[60] = 'q';A[61] = 'b';A[62] = 'n';A[63] = 'r';
}
void print_board( char A[], int s )
{
for(int a=1; a<=8;a++)
    {
    cout << " " << a << "  ";
    }
    cout << endl;
    cout << endl;
    int i,a=8;
    for (i=0; i<(s*s); i++)
    {
       cout << "_" << A[i] << "_|";
       if((i+1)%s==0)
       {
           cout << "  " << a << endl;
           cout << endl;
           a=a+8;
       }
    }
}
bool valid_move ( char piece, bool turn )
{
if (turn == 0)
    {
        return ((piece >= 'a' && piece <= 'z') || ( piece == 'k'));
    }
    else
    {
        return ((piece >= 'A' && piece <= 'Z') || ( piece == 'K'));
    }
}
bool knight_move(char piece, int from, int to, char A[])
{
    if (piece == 'N')
    {
       if((A[(from - 1) - 15] == ' ' || A[(from - 1) + 15] == ' ') && (A[to - 1] != 'N' &&  A[to - 1] != 'K' && A[to - 1] != 'Q' && A[to - 1] != 'P' && A[to - 1] != 'B' && A[to - 1] != 'R' ))
       {
       int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
       int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
       return (r_diff == 1 && c_diff == 2) || (r_diff == 2 && c_diff == 1);
       }
    }
    else if (piece == 'n')
    {
       if((A[(from - 1) - 15] == ' ' || A[(from - 1)+ 10] == ' ') && (A[to - 1] != 'n' &&  A[to - 1] != 'k' && A[to - 1] != 'q' && A[to - 1] != 'p' && A[to - 1] != 'b' && A[to - 1] != 'r' ))
       {
       int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
       int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
       return (r_diff == 1 && c_diff == 2) || (r_diff == 2 && c_diff == 1);
       }
    }
    return false;
}
bool rook_move(char piece, int from, int to, char A[])
{
    if (piece == 'r')
    {
        if((A[from - 1] == ' ' || A[from + 1] == ' ' || A[from - 1] - A[to - 1] == 8 || (A[to - 1] == ' ' && A[from - 9] == ' ')) && (A[to - 1] != 'n' &&  A[to - 1] != 'k' && A[to - 1] != 'q' && A[to - 1] != 'p' && A[to - 1] != 'b' && A[to - 1] != 'r' ))
        {
        int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
        int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
        return (r_diff == 0 || c_diff == 0);
        }
    }
    else if (piece == 'R')
    {
        if((A[from - 1] == ' ' || A[from + 1] == ' ' || A[from - 1] - A[to - 1] == 8 || (A[to - 1] == ' ' && A[from + 9] == ' ')) && (A[to - 1] != 'N' &&  A[to - 1] != 'K' && A[to - 1] != 'Q' && A[to - 1] != 'P' && A[to - 1] != 'B' && A[to - 1] != 'R' ))
        {
        int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
        int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
        return (r_diff == 0 || c_diff == 0);
        }
    }
    return false;
}
bool bishop_move(char piece, int from, int to, char A[])
{
    if (piece == 'b')
    {
    if((A[from - 1] == ' ' || A[from + 1] == ' ' || A[from - 1] - A[to - 1] == 8 || (A[(from - 1) - 7] == ' ' && A[(to - 1) + 7] == ' ') || (A[(from - 1) - 7] == ' ' && A[to - 1] == ' ' ) || (A[(from - 1) - 9] == ' ' && A[to - 1] == ' ' ) || (A[(from - 1) - 9] == ' ' && A[(to - 1) + 9] == ' ')) && (A[to - 1] != 'n' &&  A[to - 1] != 'k' && A[to - 1] != 'q' && A[to - 1] != 'p' && A[to - 1] != 'b' && A[to - 1] != 'r' ))
        {
        int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
        int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
        return (r_diff == c_diff);
        }
    }
    else  if (piece == 'B')
    {
        if((A[from - 1] == ' ' || A[from + 1] == ' ' || A[from - 1] - A[to - 1] == 8 || (A[(from - 1) + 7] == ' ' && A[(to - 1) - 7] == ' ') || (A[(from - 1) + 7] == ' ' && A[to - 1] == ' ' ) || (A[(from - 1) + 9] == ' ' && A[to - 1] == ' ' ) || (A[(from - 1) + 9] == ' ' && A[(to - 1) - 9] == ' ')) && (A[to - 1] != 'N' &&  A[to - 1] != 'K' && A[to - 1] != 'Q' && A[to - 1] != 'P' && A[to - 1] != 'B' && A[to - 1] != 'R' ))
        {
        int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
        int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
        return (r_diff == c_diff);
        }
    }
    return false;
}
bool queen_move(char piece, int from, int to, char A[])
{
    if (piece == 'Q')
    {
        if((A[from - 1] == ' ' || A[from + 1] == ' ' || A[from - 1] - A[to - 1] == 8 || (A[to - 1] == ' ' && A[from + 9] == ' ') || (A[(from - 1) - 7] == ' ' && A[(to - 1) + 7] == ' ') || (A[(from - 1) + 7] == ' ' && A[to - 1] == ' ' ) || (A[(from - 1) + 9] == ' ' && A[to - 1] == ' ' ) || (A[(from - 1) - 9] == ' ' && A[(to - 1) + 9] == ' ')) && (A[to - 1] != 'N' &&  A[to - 1] != 'K' && A[to - 1] != 'Q' && A[to - 1] != 'P' && A[to - 1] != 'B' && A[to - 1] != 'R' ))
        {
        int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
        int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
        return (r_diff == c_diff || r_diff == 0 || c_diff == 0);
        }
    }
    else  if (piece == 'q')
    {
        if((A[from - 1] == ' ' || A[from + 1] == ' ' || A[from - 1] - A[to - 1] == 8 || (A[to - 1] == ' ' && A[from - 9] == ' ') || (A[(from - 1) + 7] == ' ' && A[(to - 1) - 7] == ' ') || (A[(from - 1) - 7] == ' ' && A[to - 1] == ' ' ) || (A[(from - 1) - 9] == ' ' && A[to - 1] == ' ' ) || (A[(from - 1) + 9] == ' ' && A[(to - 1) - 9] == ' ')) && (A[to - 1] != 'n' &&  A[to - 1] != 'k' && A[to - 1] != 'q' && A[to - 1] != 'p' && A[to - 1] != 'b' && A[to - 1] != 'r' ))
        {
        int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
        int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
        return (r_diff == c_diff || r_diff == 0 || c_diff == 0);
        }
    }
    return false;
}
bool king_move(char piece, int from, int to, char A[])
{
    if (piece == 'k')
    {
        if((A[from - 1] == ' ' || A[from + 1] == ' ' || A[from - 1] - A[to - 1] == 8 || A[to - 1] == ' ') && (A[to - 1] != 'n' &&  A[to - 1] != 'k' && A[to - 1] != 'q' && A[to - 1] != 'p' && A[to - 1] != 'b' && A[to - 1] != 'r' ))
        {
        int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
        int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
        return (r_diff <= 1 && c_diff <= 1);
        }
    }
    else if (piece == 'K')
    {
        if((A[from - 1] == ' ' || A[from + 1] == ' ' || A[from - 1] - A[to - 1] == 8 || A[to - 1] == ' ') && (A[to - 1] != 'N' &&  A[to - 1] != 'K' && A[to - 1] != 'Q' && A[to - 1] != 'P' && A[to - 1] != 'B' && A[to - 1] != 'R' ))
        {
        int r_diff = abs((to - 1) / 8 - (from - 1) / 8);
        int c_diff = abs((to - 1) % 8 - (from - 1) % 8);
        return (r_diff <= 1 && c_diff <= 1);
        }
    }
    return false;
}
bool pawn_move(char piece, int from, int to, char A[])
{
    if (piece == 'P')
    {
        if ((to - from == 8) && A[to - 1] == ' ')
        {
            return true;
        } else if ((to - from == 16) && (from <= 16) && A[to - 1] == ' ' && A[to - 9] == ' ')
        {
            return true;
        } else if ((to - from == 9 || to - from == 7) && A[to - 1] >= 'a' && A[to - 1] <= 'z')
        {
            return true;
        }
    }
    else if (piece == 'p')
    {
        if ((from - to == 8) && A[to - 1] == ' ')
        {
            return true;
        } else if ((from - to == 16) && (from >= 49) && A[to - 1] == ' ' && A[to + 7] == ' ')
        {
            return true;
        } else if ((from - to == 9 || from - to == 7) && A[to - 1] >= 'A' && A[to - 1] <= 'Z')
        {
            return true;
        }
    }
    return false;
}
void come_on(int s, char A[], bool turn)
{
while (true)
    {
        int from, to;
        cout << "enter the position to move from: ";
        cin >> from;
        cout << "enter the position to move to: ";
        cin >> to;
        cout << endl;
        if (from >= 1 && from <= 64 && to >= 1 && to <= 64)
        {
            char piece= A[from - 1];
            if ((valid_move(piece, turn) && pawn_move(piece,from,to,A)) || king_move(piece, from, to, A) || queen_move(piece, from, to, A) || bishop_move(piece, from, to, A) || rook_move(piece,from,to,A) || knight_move(piece,from,to,A))
            {
            A[from - 1] = ' ';
            A[to - 1] = piece;
            print_board(A, s);
            }
            else
            {
            cout << "WARNING !!!" << endl;
            cout << "please enter the correct positions " << endl;
            cout << endl;
            come_on(s,A,turn);
            }
        }
        else
        {
            cout << "Invalid move! Please enter positions between 1 and 64." << endl;
        }
            cout << endl;
            turn_change(turn);
    }
}
int main()
{
    int s=8;
    char A[s*s];
    bool turn=0;
    cout << endl;
    cout << "press 1 to continue ..." << endl;
    int choice;
    cin >> choice;
    cout << endl;
    if (choice == 1)
    {
    init(A,s);
    }
    cout << "welcome and pay thanx to nayar !" << endl;
    cout << endl;
    cout << "player 1 = small pieces" << endl;
    cout << "player 2 = large pieces" << endl;
    cout << endl;
    print_board (A,s);
    cout << endl;
    cout << "player 1 turn" << endl;
    come_on(s,A,turn);
    return 0;
}
