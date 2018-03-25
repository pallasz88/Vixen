#include "MoveGenerator.h"
#include "Board.h"
#include "defs.h"

#include <iostream>

using namespace std;
using namespace Vixen;

MoveGenerator* MoveGenerator::instance = NULL;

MoveGenerator::MoveGenerator()
{
    GeneratePawnMoves();
    GenerateKnightMoves();
}

MoveGenerator* MoveGenerator::GetInstance()
{
    if(!instance)
        instance = new MoveGenerator();

    return instance;
}

void MoveGenerator::CleanUp()
{
    if(instance)
        delete instance;
}

void MoveGenerator::GeneratePawnMoves()
{
    BitBoards board = Board::GetInstance()->GetBitBoards();
    for(int square = H1; square < A8; ++square)
    {
        if((board.P >> square) & 1)
        {
            if( (~board.occupied) >> (square+8) & 1 )
                moveList.push_back(squares[square] + squares[square+8]);

            if( ((board.P & RANK2) >> square) & 1 && (~board.occupied) >> (square+16) & 1 )
                moveList.push_back(squares[square] + squares[square+16]);

            if( board.black >> (square+7) & 1 )
                moveList.push_back(squares[square] + squares[square+7]);

            if( board.black >> (square+9) & 1 )
                moveList.push_back(squares[square] + squares[square+9]);
        }


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
        }
    }
    return;
}

void MoveGenerator::GenerateKnightMoves()
{
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
    PrintMoveList();
    return;
}


void MoveGenerator::PrintMoveList()
{
    for(auto& x : moveList)
        cout << x << endl;
}
