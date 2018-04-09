#include "MoveGenerator.h"
#include "Board.h"
#include "defs.h"

#include <iostream>

Vixen::MoveGenerator* Vixen::MoveGenerator::instance = NULL;

Vixen::MoveGenerator::MoveGenerator()
{
    boost::thread pawn(GeneratePawnMoves, this);
    boost::thread knight(GenerateKnightMoves, this);
    pawn.join();
    knight.join();
    //PrintMoveList();
}

Vixen::MoveGenerator* Vixen::MoveGenerator::GetInstance()
{
    if(!instance)
        instance = new MoveGenerator();

    return instance;
}

void Vixen::MoveGenerator::CleanUp()
{
    if(instance)
        delete instance;

    instance = nullptr;
}

void Vixen::MoveGenerator::GeneratePawnMoves()
{
    boost::lock_guard<boost::mutex> guard(mutex);
    BitBoards board = Board::GetInstance()->GetBitBoards();
    std::string enPassantSquare = Vixen::Board::GetInstance()->GetEnPassant();
    for(int square = H1; square < A8; ++square)
    {
        if( Vixen::Board::GetInstance()->IsWhiteToMove() )
        {
            if((board.P >> square) & 1 )
            {
                if( (~board.occupied) >> (square+8) & 1 )
                    moveList.push_back(squares[square] + squares[square+8]);

                if( ((board.P & RANK2) >> square) & 1 && (~board.occupied) >> (square+16) & 1 )
                    moveList.push_back(squares[square] + squares[square+16]);

                if( board.black >> (square+7) & 1 )
                    moveList.push_back(squares[square] + squares[square+7]);

                if( board.black >> (square+9) & 1 )
                    moveList.push_back(squares[square] + squares[square+9]);

                if( enPassantSquare != "-" )
                    moveList.push_back(squares[square] + enPassantSquare);
            }
        }
        else
        {
            if((board.p >> square) & 1)
            {
                if( (~board.occupied) >> (square-8) & 1 )
                    moveList.push_back(squares[square] + squares[square-8]);

                if( ((board.p & RANK7) >> square) & 1 && (~board.occupied) >> (square-16) & 1 )
                    moveList.push_back(squares[square] + squares[square-16]);

                if( board.white >> (square-7) & 1 )
                    moveList.push_back(squares[square] + squares[square-7]);

                if( board.white >> (square-9) & 1 )
                    moveList.push_back(squares[square] + squares[square-9]);

                if( enPassantSquare != "-" )
                    moveList.push_back(squares[square] + enPassantSquare);
            }
        }
    }

    return;
}

void Vixen::MoveGenerator::GenerateKnightMoves()
{
    boost::lock_guard<boost::mutex> guard(mutex);
    BitBoards board = Board::GetInstance()->GetBitBoards();
    for(int square = H1; square < A8; ++square)
    {
        if((board.N >> square) & 1)
        {
            if( ~board.white >> (square + 15) & 1)
                moveList.push_back(squares[square] + squares[square+15]);

            if( ~board.white >> (square + 17) & 1)
                moveList.push_back(squares[square] + squares[square+17]);

            if( ~board.white >> (square - 15) & 1 )
                if( ((board.N ^ (RANK1 | RANK2)) >> square) & 1 )
                    moveList.push_back(squares[square] + squares[square-15]);

            if( ~board.white >> (square - 17) & 1 )
                if( ((board.N ^ (RANK1 | RANK2)) >> square) & 1 )
                    moveList.push_back(squares[square] + squares[square-17]);
        }


        if((board.n >> square) & 1)
        {

        }
    }
    return;
}


void Vixen::MoveGenerator::PrintMoveList()
{
    for(auto& x : moveList)
        std::cout << x << std::endl;
}
