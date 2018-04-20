#include "MoveGenerator.h"
#include "Board.h"
#include "defs.h"

#include <iostream>

Vixen::MoveGenerator::MoveGenerator(const Vixen::Board& board)
{
    GeneratePawnMoves(board);
    GenerateKnightMoves(board);
    GenerateBishopMoves(board);
    GenerateRookMoves(board);
    GenerateQueenMoves(board);
    GenerateKingMoves(board);
#ifdef DEBUG
    PrintMoveList();
#endif // DEBUG
}

void Vixen::MoveGenerator::GeneratePawnMoves(const Vixen::Board& board)
{
    BitBoards bitBoards = board.GetBitBoards();
    std::string enPassantSquare = board.GetEnPassant();
    for(int square = H2; square <= A7; ++square)
    {
        if( board.IsWhiteToMove() )
        {
            if((bitBoards.P >> square) & 1 )
            {
                if( (~bitBoards.occupied) >> (square+8) & 1 )
                {
                    moveList.push_back(squares[square] + squares[square+8]);
                    if( ((bitBoards.P & RANK2) >> square) & 1 && (~bitBoards.occupied) >> (square+16) & 1 )
                        moveList.push_back(squares[square] + squares[square+16]);

                }

                if( bitBoards.black >> (square+7) & 1 && !(((bitBoards.P & FILEH) >> square ) & 1) )
                    moveList.push_back(squares[square] + squares[square+7]);

                if( bitBoards.black >> (square+9) & 1 && !(((bitBoards.P & FILEA) >> square ) & 1) )
                    moveList.push_back(squares[square] + squares[square+9]);

                if( enPassantSquare != "-" )
                    moveList.push_back(squares[square] + enPassantSquare);
            }
        }
        else
        {
            if((bitBoards.p >> square) & 1)
            {
                if( (~bitBoards.occupied) >> (square-8) & 1 )
                {
                    moveList.push_back(squares[square] + squares[square-8]);
                    if( ((bitBoards.p & RANK7) >> square) & 1 && (~bitBoards.occupied) >> (square-16) & 1 )
                        moveList.push_back(squares[square] + squares[square-16]);

                }

                if( bitBoards.white >> (square-7) & 1 && !(((bitBoards.p & FILEA) >> square ) & 1) )
                    moveList.push_back(squares[square] + squares[square-7]);

                if( bitBoards.white >> (square-9) & 1 && !(((bitBoards.p & FILEH) >> square ) & 1) )
                    moveList.push_back(squares[square] + squares[square-9]);

                if( enPassantSquare != "-" )
                    moveList.push_back(squares[square] + enPassantSquare);
            }
        }
    }

    return;
}

void Vixen::MoveGenerator::GenerateKnightMoves(const Vixen::Board& board)
{
    BitBoards bitBoards = board.GetBitBoards();
    for(int square = H1; square <= A8; ++square)
    {
        if( board.IsWhiteToMove() )
        {
            if((bitBoards.N >> square) & 1)
            {
                if( ~bitBoards.white >> (square + 15) & 1)
                    if( ((bitBoards.N ^ (RANK78 | FILEH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+15]);

                if( ~bitBoards.white >> (square + 17) & 1)
                    if( ((bitBoards.N ^ (RANK78 | FILEA)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+17]);

                if( ~bitBoards.white >> (square - 15) & 1 )
                    if( ((bitBoards.N ^ (RANK12 | FILEA)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-15]);

                if( ~bitBoards.white >> (square - 17) & 1 )
                    if( ((bitBoards.N ^ (RANK12 | FILEH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-17]);

                if( ~bitBoards.white >> (square + 6) & 1)
                    if( ((bitBoards.N ^ (RANK8 | FILEGH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+6]);

                if( ~bitBoards.white >> (square + 10) & 1)
                    if( ((bitBoards.N ^ (RANK8 | FILEAB)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+10]);

                if( ~bitBoards.white >> (square - 6) & 1)
                    if( ((bitBoards.N ^ (RANK1 | FILEAB)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-6]);

                if( ~bitBoards.white >> (square - 10) & 1)
                    if( ((bitBoards.N ^ (RANK1 | FILEGH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-10]);
            }
        }
        else
        {
            if((bitBoards.n >> square) & 1)
            {
                if( ~bitBoards.black >> (square + 15) & 1)
                    if( ((bitBoards.n ^ (RANK78 | FILEH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+15]);

                if( ~bitBoards.black >> (square + 17) & 1)
                    if( ((bitBoards.n ^ (RANK78 | FILEA)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+17]);

                if( ~bitBoards.black >> (square - 15) & 1 )
                    if( ((bitBoards.n ^ (RANK12 | FILEA)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-15]);

                if( ~bitBoards.black >> (square - 17) & 1 )
                    if( ((bitBoards.n ^ (RANK12 | FILEH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-17]);

                if( ~bitBoards.black >> (square + 6) & 1)
                    if( ((bitBoards.n ^ (RANK8 | FILEGH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+6]);

                if( ~bitBoards.black >> (square + 10) & 1)
                    if( ((bitBoards.n ^ (RANK8 | FILEAB)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+10]);

                if( ~bitBoards.black >> (square - 6) & 1)
                    if( ((bitBoards.n ^ (RANK1 | FILEAB)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-6]);

                if( ~bitBoards.black >> (square - 10) & 1)
                    if( ((bitBoards.n ^ (RANK1 | FILEGH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-10]);
            }
        }
    }
    return;
}

void Vixen::MoveGenerator::GenerateBishopMoves(const Board& board)
{
    BitBoards bitBoards = board.GetBitBoards();
    for(int square = H1; square <= A8; ++square)
    {
        if( board.IsWhiteToMove() )
        {
            if((bitBoards.B >> square) & 1)
            {
                if(square < 55 && (BIT(square) & FILEA) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square+9*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+9*i]);
                        if( (BIT(square+9*i) & (FILEA | RANK8 | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square < 56 && (BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square+7*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+7*i]);
                        if( (BIT(square+7*i) & (FILEH | RANK8 | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square > 7 && (BIT(square) & FILEA) == 0 )
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square-7*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-7*i]);
                        if( (BIT(square-7*i) & (FILEA | RANK1 | bitBoards.black ) ) > 0)
                            break;

                        else
                            ++i;
                    }
                }

                if(square > 8 && (BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square-9*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-9*i]);
                        if( (BIT(square-9*i) & (FILEH | RANK1 | bitBoards.black)) > 0)
                            break;

                        else
                            ++i;
                    }
                }
            }
        }
        else
        {
            if((bitBoards.b >> square) & 1)
            {
                if(square < 55 && (BIT(square) & FILEA) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square+9*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+9*i]);
                        if( (BIT(square+9*i) & (FILEA | RANK8 | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square < 56 && (BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square+7*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+7*i]);
                        if( (BIT(square+7*i) & (FILEH | RANK8 | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square > 7 && (BIT(square) & FILEA) == 0 )
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square-7*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-7*i]);
                        if( (BIT(square-7*i) & (FILEA | RANK1 | bitBoards.white ) ) > 0)
                            break;

                        else
                            ++i;
                    }
                }

                if(square > 8 && (BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square-9*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-9*i]);
                        if( (BIT(square-9*i) & (FILEH | RANK1 | bitBoards.white)) > 0)
                            break;

                        else
                            ++i;
                    }
                }
            }
        }
    }
}

void Vixen::MoveGenerator::GenerateRookMoves(const Board& board)
{
    Vixen::BitBoards bitBoards = board.GetBitBoards();
    for(int square = H1; square <= A8; ++square)
    {
        if( board.IsWhiteToMove() )
        {
            if((bitBoards.R >> square) & 1)
            {
                if((BIT(square) & RANK8) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square+8*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+8*i]);
                        if( (BIT(square+8*i) & (RANK8 | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & FILEA) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square+1*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+1*i]);
                        if( (BIT(square+1*i) & (FILEA | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square-1*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-1*i]);
                        if( (BIT(square-1*i) & (FILEH | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & RANK1) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square-8*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-8*i]);
                        if( (BIT(square-8*i) & (RANK1 | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }
            }
        }
        else
        {
            if((bitBoards.r >> square) & 1)
            {
                if((BIT(square) & RANK8) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square+8*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+8*i]);
                        if( (BIT(square+8*i) & (RANK8 | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & FILEA) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square+1*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+1*i]);
                        if( (BIT(square+1*i) & (FILEA | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square-1*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-1*i]);
                        if( (BIT(square-1*i) & (FILEH | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & RANK1) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square-8*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-8*i]);
                        if( (BIT(square-8*i) & (RANK1 | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }
            }
        }
    }
}

void Vixen::MoveGenerator::GenerateQueenMoves(const Board& board)
{
    Vixen::BitBoards bitBoards = board.GetBitBoards();
    for(int square = H1; square <= A8; ++square)
    {
        if( board.IsWhiteToMove() )
        {
            if((bitBoards.Q >> square) & 1)
            {
                if((BIT(square) & RANK8) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square+8*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+8*i]);
                        if( (BIT(square+8*i) & (RANK8 | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & FILEA) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square+1*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+1*i]);
                        if( (BIT(square+1*i) & (FILEA | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square-1*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-1*i]);
                        if( (BIT(square-1*i) & (FILEH | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & RANK1) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square-8*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-8*i]);
                        if( (BIT(square-8*i) & (RANK1 | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square < 55 && (BIT(square) & FILEA) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square+9*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+9*i]);
                        if( (BIT(square+9*i) & (FILEA | RANK8 | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square < 56 && (BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square+7*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+7*i]);
                        if( (BIT(square+7*i) & (FILEH | RANK8 | bitBoards.black)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square > 7 && (BIT(square) & FILEA) == 0 )
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square-7*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-7*i]);
                        if( (BIT(square-7*i) & (FILEA | RANK1 | bitBoards.black ) ) > 0)
                            break;

                        else
                            ++i;
                    }
                }

                if(square > 8 && (BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.white) >> (square-9*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-9*i]);
                        if( (BIT(square-9*i) & (FILEH | RANK1 | bitBoards.black)) > 0)
                            break;

                        else
                            ++i;
                    }
                }
            }
        }
        else
        {
            if((bitBoards.q >> square) & 1)
            {
                if((BIT(square) & RANK8) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square+8*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+8*i]);
                        if( (BIT(square+8*i) & (RANK8 | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & FILEA) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square+1*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+1*i]);
                        if( (BIT(square+1*i) & (FILEA | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square-1*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-1*i]);
                        if( (BIT(square-1*i) & (FILEH | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if((BIT(square) & RANK1) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square-8*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-8*i]);
                        if( (BIT(square-8*i) & (RANK1 | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square < 55 && (BIT(square) & FILEA) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square+9*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+9*i]);
                        if( (BIT(square+9*i) & (FILEA | RANK8 | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square < 56 && (BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square+7*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square+7*i]);
                        if( (BIT(square+7*i) & (FILEH | RANK8 | bitBoards.white)) > 0 )
                            break;

                        else
                            ++i;
                    }
                }

                if(square > 7 && (BIT(square) & FILEA) == 0 )
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square-7*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-7*i]);
                        if( (BIT(square-7*i) & (FILEA | RANK1 | bitBoards.white ) ) > 0)
                            break;

                        else
                            ++i;
                    }
                }

                if(square > 8 && (BIT(square) & FILEH) == 0)
                {
                    unsigned i = 1;
                    while( (~bitBoards.black) >> (square-9*i) & 1 )
                    {
                        moveList.push_back(squares[square] + squares[square-9*i]);
                        if( (BIT(square-9*i) & (FILEH | RANK1 | bitBoards.white)) > 0)
                            break;

                        else
                            ++i;
                    }
                }
            }
        }
    }
}

void Vixen::MoveGenerator::GenerateKingMoves(const Board& board)
{
    Vixen::BitBoards bitBoards = board.GetBitBoards();
    for(int square = H1; square <= A8; ++square)
    {
        if( board.IsWhiteToMove() )
        {
            if((bitBoards.K >> square) & 1 )
            {
                if( (BIT(square + 8) & (~bitBoards.white)) > 0 && (BIT(square) & RANK8) == 0 )
                    moveList.push_back(squares[square] + squares[square+8]);

                if( (BIT(square + 1) & (~bitBoards.white)) > 0 && (BIT(square) & FILEA) == 0 )
                    moveList.push_back(squares[square] + squares[square+1]);

                if( (BIT(square - 1) & (~bitBoards.white)) > 0 && (BIT(square) & FILEH) == 0 )
                    moveList.push_back(squares[square] + squares[square-1]);

                if( (BIT(square - 8) & (~bitBoards.white)) > 0 && (BIT(square) & RANK1) == 0 )
                    moveList.push_back(squares[square] + squares[square-8]);

                if( (BIT(square + 9) & (~bitBoards.white)) > 0 && (BIT(square) & (RANK8 | FILEA)) == 0 )
                    moveList.push_back(squares[square] + squares[square+9]);

                if( (BIT(square + 7) & (~bitBoards.white)) > 0 && (BIT(square) & (RANK8 | FILEH)) == 0 )
                    moveList.push_back(squares[square] + squares[square+7]);

                if( (BIT(square - 7) & (~bitBoards.white)) > 0 && (BIT(square) & (RANK1 | FILEA)) == 0 )
                    moveList.push_back(squares[square] + squares[square-7]);

                if( (BIT(square - 9) & (~bitBoards.white)) > 0 && (BIT(square) & (RANK1 | FILEH)) == 0 )
                    moveList.push_back(squares[square] + squares[square-9]);
            }
        }
        else
        {
            if((bitBoards.k >> square) & 1 )
            {
                if( (BIT(square + 8) & (~bitBoards.black)) > 0 && (BIT(square) & RANK8) == 0 )
                    moveList.push_back(squares[square] + squares[square+8]);

                if( (BIT(square + 1) & (~bitBoards.black)) > 0 && (BIT(square) & FILEA) == 0 )
                    moveList.push_back(squares[square] + squares[square+1]);

                if( (BIT(square - 1) & (~bitBoards.black)) > 0 && (BIT(square) & FILEH) == 0 )
                    moveList.push_back(squares[square] + squares[square-1]);

                if( (BIT(square - 8) & (~bitBoards.black)) > 0 && (BIT(square) & RANK1) == 0 )
                    moveList.push_back(squares[square] + squares[square-8]);

                if( (BIT(square + 9) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK8 | FILEA)) == 0 )
                    moveList.push_back(squares[square] + squares[square+9]);

                if( (BIT(square + 7) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK8 | FILEH)) == 0 )
                    moveList.push_back(squares[square] + squares[square+7]);

                if( (BIT(square - 7) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK1 | FILEA)) == 0 )
                    moveList.push_back(squares[square] + squares[square-7]);

                if( (BIT(square - 9) & (~bitBoards.black)) > 0 && (BIT(square) & (RANK1 | FILEH)) == 0 )
                    moveList.push_back(squares[square] + squares[square-9]);
            }
        }
    }
}

void Vixen::MoveGenerator::PrintMoveList()
{
    for(auto& x : moveList)
        std::cout << x << std::endl;
}
