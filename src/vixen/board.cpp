#include "board.h"
#include "move_generator.h"
#include "hash.h"
#include "defs.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <ctime>

Vixen::Board::Board()
{
#ifdef DEBUG
    clock_t start = clock();
#endif
    SetBoard(STARTPOS);
#ifdef DEBUG
    PrintBoard();
    std::cout << double( clock() - start ) / (double)CLOCKS_PER_SEC << " seconds." << std::endl;
#endif
}

void Vixen::Board::SetBoard(const std::string& fenPosition)
{
    this->fenPosition = fenPosition;
    InitBitBoards(bitBoards);
    std::vector<std::string> parsedPosition;
    SplitFenPosition(parsedPosition);
    ParseFenPiecePart(parsedPosition[0]);
    ParseSideToMovePart(parsedPosition[1]);
    ParseCastlingRightPart(parsedPosition[2]);
    enpassant = parsedPosition[3];
    fiftyMoves = stoi(parsedPosition[4]);
    fullMovesNum = stoi(parsedPosition[5]);
    generator.reset();
    hashBoard.reset();
#ifdef __cplusplus_201402L
    generator = std::make_unique<Vixen::MoveGenerator>(*this);
    hashBoard = std::make_unique<Vixen::Hash>(*this);
#else
	generator = std::make_shared<Vixen::MoveGenerator>(*this);
	hashBoard = std::make_shared<Vixen::Hash>(*this);
#endif
}

void Vixen::Board::SplitFenPosition(std::vector<std::string>& fenParts)
{
    boost::split(fenParts, fenPosition, boost::is_any_of(" "));
}

void Vixen::Board::PrintBoard() const
{
    std::cout << std::endl << "*********************************" << std::endl;
    for (int i = MAX_SHIFT_NUM; i >= 0; i--)
    {
        if ((~bitBoards.occupied >> i)&1)
            std::cout << "|   ";

        else if ((bitBoards.P >> i)&1)
            std::cout << "| P ";

        else if((bitBoards.p >> i)&1)
            std::cout << "| p ";

        else if((bitBoards.K >> i)&1)
            std::cout << "| K ";

        else if((bitBoards.k >> i)&1)
            std::cout << "| k ";

        else if((bitBoards.Q >> i)&1)
            std::cout << "| Q ";

        else if((bitBoards.q >> i)&1)
            std::cout << "| q ";

        else if((bitBoards.R >> i)&1)
            std::cout << "| R ";

        else if((bitBoards.r >> i)&1)
            std::cout << "| r ";

        else if((bitBoards.N >> i)&1)
            std::cout << "| N ";

        else if((bitBoards.n >> i)&1)
            std::cout << "| n ";

        else if((bitBoards.B >> i)&1)
            std::cout << "| B ";

        else if((bitBoards.b >> i)&1)
            std::cout << "| b ";

        if (i%8 == 0)
        {
            std::cout << "| " << i/8 + 1 << std::endl;
            std::cout << "*********************************" << std::endl;
        }
    }

    std::cout << "  ";
    for (int i = 0; i < 8; ++i)
    {
        std::cout << static_cast<char>('a' + i) << "   ";
    }
    std::cout << std::endl << std::endl;
}

void Vixen::Board::ParseFenPiecePart(const std::string& parsedPosition)
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
            std::cerr << "ERROR IN FEN: PIECE POSITION" << std::endl;
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

void Vixen::Board::ParseSideToMovePart(const std::string& parsedPosition)
{
    if( parsedPosition == "w" )
        whiteToMove = true;

    else if( parsedPosition == "b" )
        whiteToMove = false;

    else
        std::cerr << "ERROR IN FEN: SIDE TO MOVE" << std::endl;

}

void Vixen::Board::ParseCastlingRightPart(const std::string& parsedPosition)
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
            std::cerr << "ERROR IN FEN: CASTLING" << std::endl;
            break;
        }
    }
}

void Vixen::InitBitBoards(BitBoards& bitBoards)
{
    memset( &bitBoards, 0x00, sizeof( bitBoards ) );
}

bool Vixen::IsSquareWhiteControlled(BitBoards& bitBoards, int square)
{
    if(square < A7 && (BIT(square) & FILEA) == 0)
    {
        unsigned i = 1;
        while( (~bitBoards.black) >> (square+9*i) & 1 )
        {
            if( (BIT(square+9*i) & (FILEA | RANK8 | bitBoards.black)) > 0 )
                break;

            else if( (BIT(square+9*i) & bitBoards.white) > 0)
                return true;

            else
                ++i;
        }
    }

    if(square < H8 && (BIT(square) & FILEH) == 0)
    {
        unsigned i = 1;
        while( (~bitBoards.black) >> (square+7*i) & 1 )
        {
            if( (BIT(square+7*i) & (FILEH | RANK8 | bitBoards.black)) > 0 )
                break;

            else if( (BIT(square+7*i) & (bitBoards.Q | bitBoards.B )) > 0)
                return true;

            else
                ++i;
        }
    }

    if(square > A1 && (BIT(square) & FILEA) == 0 )
    {
        unsigned i = 1;
        while( (~bitBoards.black) >> (square-7*i) & 1 )
        {
            if( (BIT(square-7*i) & (FILEA | RANK1 | bitBoards.black ) ) > 0)
                break;

            else if( (BIT(square-7*i) & (bitBoards.Q | bitBoards.B )) > 0)
                return true;

            else
                ++i;
        }
    }

    if(square > H2 && (BIT(square) & FILEH) == 0)
    {
        unsigned i = 1;
        while( (~bitBoards.black) >> (square-9*i) & 1 )
        {
            if( (BIT(square-9*i) & (FILEH | RANK1 | bitBoards.black)) > 0)
                break;

            else if( (BIT(square-9*i) & (bitBoards.Q | bitBoards.B )) > 0)
                return true;

            else
                ++i;
        }
    }

    if((BIT(square) & RANK8) == 0)
    {
        unsigned i = 1;
        while( (~bitBoards.black) >> (square+8*i) & 1 )
        {
            if( (BIT(square+8*i) & (RANK8 | bitBoards.black)) > 0 )
                break;

            else if( (BIT(square+8*i) & (bitBoards.Q | bitBoards.R )) > 0)
                return true;

            else
                ++i;
        }
    }

    if((BIT(square) & FILEA) == 0)
    {
        unsigned i = 1;
        while( (~bitBoards.black) >> (square+1*i) & 1 )
        {
            if( (BIT(square+1*i) & (FILEA | bitBoards.black)) > 0 )
                break;

            else if( (BIT(square+1*i) & (bitBoards.Q | bitBoards.R )) > 0)
                return true;

            else
                ++i;
        }
    }

    if((BIT(square) & FILEH) == 0)
    {
        unsigned i = 1;
        while( (~bitBoards.black) >> (square-1*i) & 1 )
        {
            if( (BIT(square-1*i) & (FILEH | bitBoards.black)) > 0 )
                break;

            else if( (BIT(square-1*i) & (bitBoards.Q | bitBoards.R )) > 0)
                return true;

            else
                ++i;
        }
    }

    if((BIT(square) & RANK1) == 0)
    {
        unsigned i = 1;
        while( (~bitBoards.black) >> (square-8*i) & 1 )
        {
            if( (BIT(square-8*i) & (RANK1 | bitBoards.black)) > 0 )
                break;

            else if( (BIT(square-8*i) & (bitBoards.Q | bitBoards.R )) > 0)
                return true;

            else
                ++i;
        }
    }

    if( (BIT(square + 8) & (~bitBoards.black)) > 0 && (BIT(square) & RANK8) == 0 && (bitBoards.K >> (square+8)) & 1 )
        return true;

    if( (BIT(square + 1) & (~bitBoards.black)) > 0 && (BIT(square) & FILEA) == 0 && (bitBoards.K >> (square+1)) & 1 )
        return true;

    if( (BIT(square - 1) & (~bitBoards.black)) > 0 && (BIT(square) & FILEH) == 0 && (bitBoards.K >> (square-1)) & 1 )
        return true;

    if( (BIT(square - 8) & (~bitBoards.black)) > 0 && (BIT(square) & RANK1) == 0 && (bitBoards.K >> (square-8)) & 1 )
        return true;

    if( (BIT(square + 9) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK8 | FILEA)) == 0 && (bitBoards.K >> (square+9)) & 1 )
        return true;

    if( (BIT(square + 7) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK8 | FILEH)) == 0 && (bitBoards.K >> (square+7)) & 1 )
        return true;

    if( (BIT(square - 7) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK1 | FILEA)) == 0 && (bitBoards.K >> (square-7)) & 1 )
        return true;

    if( (BIT(square - 9) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK1 | FILEH)) == 0 && (bitBoards.K >> (square-9)) & 1 )
        return true;

    if( (BIT(square + 15) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK78 | FILEH)) == 0 && (bitBoards.N >> (square+15)) & 1 )
        return true;

    if( (BIT(square + 17) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK78 | FILEA)) == 0 && (bitBoards.N >> (square+17)) & 1 )
        return true;

    if( (BIT(square - 15) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK12 | FILEA)) == 0 && (bitBoards.N >> (square-15)) & 1 )
        return true;

    if( (BIT(square - 17) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK12 | FILEH)) == 0 && (bitBoards.N >> (square-17)) & 1 )
        return true;

    if( (BIT(square + 6) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK8 | FILEGH)) == 0 && (bitBoards.N >> (square+6)) & 1 )
        return true;

    if( (BIT(square + 10) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK8 | FILEAB)) == 0 && (bitBoards.N >> (square+10)) & 1 )
        return true;

    if( (BIT(square - 6) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK1 | FILEAB)) == 0 && (bitBoards.N >> (square-6)) & 1 )
        return true;

    if( (BIT(square - 10) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK1 | FILEGH)) == 0 && (bitBoards.N >> (square-10)) & 1 )
        return true;

    return false;
}

bool Vixen::IsSquareBlackControlled()
{
	return false;
}

