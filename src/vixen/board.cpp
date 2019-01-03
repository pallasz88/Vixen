#include "board.h"
#include "move_generator.h"
#include "hash.h"

#include <iostream>
#include <boost/algorithm/string.hpp>

namespace Vixen
{
    Board::Board()
    {
        SetBoard(START_POSITION);
#ifdef DEBUG
        PrintBoard();
#endif
    }

    void Board::SetBoard(const std::string &fenPosition)
    {
        this->fenPosition = fenPosition;
        InitBitBoards(bitBoards);
        std::vector<std::string> parsedPosition;
        SplitFenPosition(parsedPosition);
        ParseFenPiecePart(parsedPosition[0]);
        ParseSideToMovePart(parsedPosition[1]);
        ParseCastlingRightPart(parsedPosition[2]);
        enPassant = SquareToBitBoard(NotationToSquare(parsedPosition[3]));
        fiftyMoves = stoi(parsedPosition[4]);
        historyMovesNum = stoi(parsedPosition[5]);
        generator = std::make_unique<MoveGenerator>(*this);
        hashBoard = std::make_unique<Hash>(*this);
    }

    void Board::SplitFenPosition(std::vector<std::string> &fenParts)
    {
        boost::split(fenParts, fenPosition, boost::is_any_of(" "));
    }

    void Board::PrintBoard() const
    {
        std::cout << std::endl << "+---+---+---+---+---+---+---+---+" << std::endl;
        for (int squareIndex = MAX_SQUARE_INDEX; squareIndex >= H1; --squareIndex)
        {
            for (const auto &item : bitBoards)
            {
                if (IsBitSet(item.second, squareIndex) && item.first != 'F' && item.first != 'S')
                {
                    std::cout << "| " << item.first << " ";
                    break;
                }
            }
            if (squareIndex % 8 == 0)
            {
                std::cout << "| " << squareIndex / 8 + 1 << std::endl;
                std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
            }
        }

        std::cout << "  ";
        for (int rank = 0; rank < 8; ++rank)
        {
            std::cout << static_cast<char>('a' + rank) << "   ";
        }
        std::cout << std::endl << std::endl;
    }

    void Board::ParseFenPiecePart(const std::string &parsedPosition)
    {
        unsigned squareIndex = MAX_SQUARE_INDEX;
        for (const auto &fenChar : parsedPosition)
        {
            switch (fenChar)
            {
                case 'P':
                case 'N':
                case 'B':
                case 'R':
                case 'Q':
                case 'K':
                case 'p':
                case 'n':
                case 'b':
                case 'r':
                case 'q':
                case 'k':
                    SetBit(bitBoards[fenChar], squareIndex);
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
                    squareIndex -= fenChar - '1';
                    break;
                default:
                    std::cerr << "ERROR IN FEN: PIECE POSITION" << std::endl;
                    return;
            }
            --squareIndex;
        }
        bitBoards['S'] = bitBoards['k'] |
                         bitBoards['q'] |
                         bitBoards['r'] |
                         bitBoards['b'] |
                         bitBoards['n'] |
                         bitBoards['p'];
        bitBoards['F'] = bitBoards['K'] |
                         bitBoards['Q'] |
                         bitBoards['R'] |
                         bitBoards['B'] |
                         bitBoards['N'] |
                         bitBoards['P'];
        bitBoards[' '] = ~(bitBoards['F'] | bitBoards['S']);
    }

    void Board::ParseSideToMovePart(const std::string &parsedPosition)
    {
        if (parsedPosition == "w")
            whiteToMove = true;

        else if (parsedPosition == "b")
            whiteToMove = false;

        else
            std::cerr << "ERROR IN FEN: SIDE TO MOVE" << std::endl;

    }

    void Board::ParseCastlingRightPart(const std::string &parsedPosition)
    {
        castlingRights = 0;
        for (const auto &it : parsedPosition)
        {
            switch (it)
            {
                case 'K':
                    SetBit(castlingRights, WKCA);
                    break;
                case 'Q':
                    SetBit(castlingRights, WQCA);
                    break;
                case 'k':
                    SetBit(castlingRights, BKCA);
                    break;
                case 'q':
                    SetBit(castlingRights, BQCA);
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

    void InitBitBoards(BitBoards &bitBoards)
    {
        bitBoards['P'] = EMPTY_BOARD;
        bitBoards['p'] = EMPTY_BOARD;
        bitBoards['B'] = EMPTY_BOARD;
        bitBoards['b'] = EMPTY_BOARD;
        bitBoards['N'] = EMPTY_BOARD;
        bitBoards['n'] = EMPTY_BOARD;
        bitBoards['K'] = EMPTY_BOARD;
        bitBoards['k'] = EMPTY_BOARD;
        bitBoards['r'] = EMPTY_BOARD;
        bitBoards['R'] = EMPTY_BOARD;
        bitBoards['Q'] = EMPTY_BOARD;
        bitBoards['q'] = EMPTY_BOARD;
        bitBoards['F'] = EMPTY_BOARD;
        bitBoards['S'] = EMPTY_BOARD;
        bitBoards[' '] = EMPTY_BOARD;
    }

}