#include "board.h"
#include "move_generator.h"
#include "hash.h"

#include <iostream>
#include <boost/algorithm/string.hpp>

namespace Vixen
{
    Board::Board()
    {
#ifdef DEBUG
        clock_t start = clock();
#endif
        SetBoard(STARTPOS);
#ifdef DEBUG
        PrintBoard();
        std::cout << double(clock() - start) / (double) CLOCKS_PER_SEC << " seconds." << std::endl;
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
        enPassant = parsedPosition[3];
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
        for (int i = MAX_SQUARE_INDEX; i >= 0; i--)
        {
            for (auto &it : bitBoards)
            {
                if (((it.second >> i) & 1) && it.first != 'F' && it.first != 'S')
                {
                    std::cout << "| " << it.first << " ";
                    break;
                }
            }
            if (i % 8 == 0)
            {
                std::cout << "| " << i / 8 + 1 << std::endl;
                std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
            }
        }

        std::cout << "  ";
        for (int i = 0; i < 8; ++i)
        {
            std::cout << static_cast<char>('a' + i) << "   ";
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
                case 'p':
                case 'N':
                case 'n':
                case 'B':
                case 'b':
                case 'R':
                case 'r':
                case 'K':
                case 'k':
                case 'Q':
                case 'q':
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
        for (auto &it : parsedPosition)
        {
            switch (it)
            {
                case 'K':
                    SetBit(castlingRights, 3);
                    break;
                case 'Q':
                    SetBit(castlingRights, 2);
                    break;
                case 'k':
                    SetBit(castlingRights, 1);
                    break;
                case 'q':
                    SetBit(castlingRights, 0);
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