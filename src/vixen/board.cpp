#include "board.h"
#include "move_generator.h"
#include "hash.h"
#include "slider.h"
#include "anti_slider.h"

#include <iostream>
#include <boost/algorithm/string.hpp>

namespace Vixen
{
    Board::Board()
    {
#ifdef DEBUG
        clock_t start = clock();
#endif
        SetBoard(TESTPOS2);
        SliderAttacks sliderAttacks(~bitBoards[' ']);
        InitKnightKingAttack();
        std::cout << sliderAttacks.GetRookAttack(E3, ~bitBoards[' ']) << std::endl;
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
        enpassant = parsedPosition[3];
        fiftyMoves = stoi(parsedPosition[4]);
        fullMovesNum = stoi(parsedPosition[5]);
        generator = std::make_unique<MoveGenerator>(*this);
        hashBoard = std::make_unique<Hash>(*this);
    }

    void Board::SplitFenPosition(std::vector<std::string> &fenParts)
    {
        boost::split(fenParts, fenPosition, boost::is_any_of(" "));
    }

    void Board::PrintBoard() const
    {
        std::cout << std::endl << "*********************************" << std::endl;
        for (int i = MAX_SHIFT_NUM; i >= 0; i--)
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

    void Board::ParseFenPiecePart(const std::string &parsedPosition)
    {
        int i = MAX_SHIFT_NUM;
        for (auto &it : parsedPosition)
        {
            switch (it)
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
                    bitBoards[it] |= static_cast<BitBoard>(1) << i;
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
                    i -= it - '0' - 1;
                    break;
                default:
                    std::cerr << "ERROR IN FEN: PIECE POSITION" << std::endl;
                    return;
            }
            --i;
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
        uint64_t shiftMe = 1;
        for (auto &it : parsedPosition)
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

    void InitBitBoards(BitBoards &bitBoards)
    {
        bitBoards['P'] = 0;
        bitBoards['p'] = 0;
        bitBoards['B'] = 0;
        bitBoards['b'] = 0;
        bitBoards['N'] = 0;
        bitBoards['n'] = 0;
        bitBoards['K'] = 0;
        bitBoards['k'] = 0;
        bitBoards['r'] = 0;
        bitBoards['R'] = 0;
        bitBoards['Q'] = 0;
        bitBoards['q'] = 0;
        bitBoards['F'] = 0;
        bitBoards['S'] = 0;
        bitBoards[' '] = 0;
    }

}