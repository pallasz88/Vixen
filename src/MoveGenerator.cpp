#include "MoveGenerator.h"
#include "Board.h"
#include "defs.h"

#include <iostream>

Vixen::MoveGenerator::MoveGenerator(const Vixen::Board& board)
{
    GeneratePawnMoves(board);
    GenerateKnightMoves(board);
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
                    moveList.push_back(squares[square] + squares[square+8]);

                if( ((bitBoards.P & RANK2) >> square) & 1 && (~bitBoards.occupied) >> (square+16) & 1 )
                    moveList.push_back(squares[square] + squares[square+16]);

                if( bitBoards.black >> (square+7) & 1 )
                    moveList.push_back(squares[square] + squares[square+7]);

                if( bitBoards.black >> (square+9) & 1 )
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
                    moveList.push_back(squares[square] + squares[square-8]);

                if( ((bitBoards.p & RANK7) >> square) & 1 && (~bitBoards.occupied) >> (square-16) & 1 )
                    moveList.push_back(squares[square] + squares[square-16]);

                if( bitBoards.white >> (square-7) & 1 )
                    moveList.push_back(squares[square] + squares[square-7]);

                if( bitBoards.white >> (square-9) & 1 )
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
    BitBoards bitboards = board.GetBitBoards();
    for(int square = H1; square <= A8; ++square)
    {
        if( board.IsWhiteToMove() )
        {
            if((bitboards.N >> square) & 1)
            {
                if( ~bitboards.white >> (square + 15) & 1)
                    if( ((bitboards.N ^ (RANK78 | FILEH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+15]);

                if( ~bitboards.white >> (square + 17) & 1)
                    if( ((bitboards.N ^ (RANK78 | FILEA)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+17]);

                if( ~bitboards.white >> (square - 15) & 1 )
                    if( ((bitboards.N ^ (RANK12 | FILEA)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-15]);

                if( ~bitboards.white >> (square - 17) & 1 )
                    if( ((bitboards.N ^ (RANK12 | FILEH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-17]);

                if( ~bitboards.white >> (square + 6) & 1)
                    if( ((bitboards.N ^ (RANK8 | FILEGH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+6]);

                if( ~bitboards.white >> (square + 10) & 1)
                    if( ((bitboards.N ^ (RANK8 | FILEAB)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+10]);

                if( ~bitboards.white >> (square - 6) & 1)
                    if( ((bitboards.N ^ (RANK1 | FILEAB)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-6]);

                if( ~bitboards.white >> (square - 10) & 1)
                    if( ((bitboards.N ^ (RANK1 | FILEGH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-10]);
            }
        }
        else
        {
            if((bitboards.n >> square) & 1)
            {
                if( ~bitboards.black >> (square + 15) & 1)
                    if( ((bitboards.n ^ (RANK78 | FILEH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+15]);

                if( ~bitboards.black >> (square + 17) & 1)
                    if( ((bitboards.n ^ (RANK78 | FILEA)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+17]);

                if( ~bitboards.black >> (square - 15) & 1 )
                    if( ((bitboards.n ^ (RANK12 | FILEA)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-15]);

                if( ~bitboards.black >> (square - 17) & 1 )
                    if( ((bitboards.n ^ (RANK12 | FILEH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-17]);

                if( ~bitboards.black >> (square + 6) & 1)
                    if( ((bitboards.n ^ (RANK8 | FILEGH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+6]);

                if( ~bitboards.black >> (square + 10) & 1)
                    if( ((bitboards.n ^ (RANK8 | FILEAB)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square+10]);

                if( ~bitboards.black >> (square - 6) & 1)
                    if( ((bitboards.n ^ (RANK1 | FILEAB)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-6]);

                if( ~bitboards.black >> (square - 10) & 1)
                    if( ((bitboards.n ^ (RANK1 | FILEGH)) >> square) & 1 )
                        moveList.push_back(squares[square] + squares[square-10]);
            }
        }
    }
    return;
}


void Vixen::MoveGenerator::PrintMoveList()
{
    for(auto& x : moveList)
        std::cout << x << std::endl;
}
