#include "Board.h"
#include "defs.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <ctime>

using namespace std;
using namespace Vixen;

Board* Board::instance = NULL;

Board::Board()
{
#ifdef DEBUG
    clock_t start = clock();
#endif
    fenPosition = STARTPOS;
    SetBoard(fenPosition);
#ifdef DEBUG
    PrintBoard();
    cout << double( clock() - start ) / (double)CLOCKS_PER_SEC << " seconds." << endl;
#endif
}

Board::~Board()
{

}

void Board::CleanUp()
{
    if(instance)
        delete instance;
}

Board* Board::GetInstance()
{
    if(!instance)
        instance = new Board;

    return instance;
}

BitBoards Board::GetBitBoards() const
{
    return bitBoards;
}

void Board::SetBoard(const string& fenPosition)
{
    this->fenPosition = fenPosition;
    InitBitBoards(bitBoards);
    vector<string> parsedPosition;
    SplitFenPosition(parsedPosition);
    ParseFenPiecePart(parsedPosition[0]);
    ParseSideToMovePart(parsedPosition[1]);
    ParseCastlingRightPart(parsedPosition[2]);
    enpassant = parsedPosition[3];
    fiftyMoves = stoi(parsedPosition[4]);
    fullMovesNum = stoi(parsedPosition[5]);
}

void Board::SplitFenPosition(vector<string>& fenParts)
{
    boost::split(fenParts, fenPosition, boost::is_any_of(" "));
}

bool Board::IsWhiteToMove() const
{
    return whiteToMove;
}

void Board::PrintBoard() const
{
    cout << endl << "*********************************" << endl;
    for (int i = MAX_SHIFT_NUM; i >= 0; i--)
    {
        if ((~bitBoards.occupied >> i)&1)
        {
            cout << "|   ";
        }
        else
        {
            if ((bitBoards.P >> i)&1)
                cout << "| P ";

            else if((bitBoards.p >> i)&1)
                cout << "| p ";

            else if((bitBoards.K >> i)&1)
                cout << "| K ";

            else if((bitBoards.k >> i)&1)
                cout << "| k ";

            else if((bitBoards.Q >> i)&1)
                cout << "| Q ";

            else if((bitBoards.q >> i)&1)
                cout << "| q ";

            else if((bitBoards.R >> i)&1)
                cout << "| R ";

            else if((bitBoards.r >> i)&1)
                cout << "| r ";

            else if((bitBoards.N >> i)&1)
                cout << "| N ";

            else if((bitBoards.n >> i)&1)
                cout << "| n ";

            else if((bitBoards.B >> i)&1)
                cout << "| B ";

            else if((bitBoards.b >> i)&1)
                cout << "| b ";


        }
        if (i%8 == 0)
        {
            cout << "| " << i/8 + 1 << endl;
            cout << "*********************************" << endl;
        }
    }

    cout << "  ";
    for (int i = 0; i < 8; ++i)
    {
        cout << static_cast<char>('a' + i) << "   ";
    }
    cout << endl << endl;
}

void Board::ParseFenPiecePart(string& parsedPosition)
{
    int i = MAX_SHIFT_NUM;
    uint64_t shiftMe = 1;
    for(auto & it : parsedPosition)
    {
        switch ( it )
        {
        case 'P':
            bitBoards.P |= shiftMe << i;
            break;
        case 'p':
            bitBoards.p |= shiftMe << i;
            break;
        case 'N':
            bitBoards.N |= shiftMe << i;
            break;
        case 'n':
            bitBoards.n |= shiftMe << i;
            break;
        case 'B':
            bitBoards.B |= shiftMe << i;
            break;
        case 'b':
            bitBoards.b |= shiftMe << i;
            break;
        case 'R':
            bitBoards.R |= shiftMe << i;
            break;
        case 'r':
            bitBoards.r |= shiftMe << i;
            break;
        case 'K':
            bitBoards.K |= shiftMe << i;
            break;
        case 'k':
            bitBoards.k |= shiftMe << i;
            break;
        case 'Q':
            bitBoards.Q |= shiftMe << i;
            break;
        case 'q':
            bitBoards.q |= shiftMe << i;
            break;
        case '/':
            continue;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            i -= it-'0'-1;
            break;
        default:
            cerr << "ERROR IN FEN: PIECE POSITION" << endl;
            return;
        }
        i--;
    }
    bitBoards.black = bitBoards.k |
                      bitBoards.q |
                      bitBoards.r |
                      bitBoards.b |
                      bitBoards.n |
                      bitBoards.p;
    bitBoards.white = bitBoards.K |
                      bitBoards.Q |
                      bitBoards.R |
                      bitBoards.B |
                      bitBoards.N |
                      bitBoards.P;
    bitBoards.occupied = bitBoards.black | bitBoards.white;
}

void Board::ParseSideToMovePart(string& parsedPosition)
{
    if( parsedPosition == "w" )
        whiteToMove = true;

    else if( parsedPosition == "b" )
        whiteToMove = false;

    else
        cerr << "ERROR IN FEN: SIDE TO MOVE" << endl;

}

void Board::ParseCastlingRightPart(string& parsedPosition)
{
    castlingRights = 0;
    uint64_t shiftMe = 1;
    for(auto & it : parsedPosition)
    {
        switch (it)
        {
        case 'K':
            castlingRights |= shiftMe << 3;
            break;
        case 'Q':
            castlingRights |= shiftMe << 2;
            break;
        case 'k':
            castlingRights |= shiftMe << 1;
            break;
        case 'q':
            castlingRights |= shiftMe;
            break;
        case '-':
            castlingRights = 0;
            break;
        default:
            cerr << "ERROR IN FEN: CASTLING" << endl;
            break;
        }
    }
}

void Vixen::InitBitBoards(BitBoards& bitBoards)
{
    memset( &bitBoards, 0x00, sizeof( bitBoards ) );
}
