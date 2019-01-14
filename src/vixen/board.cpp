#include "board.h"
#include "move_generator.h"
#include "hash.h"
#include "timer.h"

#include <iostream>
#include <bitset>
#include <boost/algorithm/string.hpp>

namespace Vixen
{
    constexpr int castlePermission[SQUARE_NUMBER] = {
            11, 15, 15, 3, 15, 15, 15, 7,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            14, 15, 15, 12, 15, 15, 15, 13
    };

    Board::Board()
    {
        SetBoard(START_POSITION);
        InitKnightKingAttack();
        InitPawnAttack();
#ifdef DEBUG
        PrintBoard();
#endif
    }

    void Board::SetBoard(const std::string &fenPosition)
    {
        this->fenPosition = fenPosition;
        InitBitBoards(bitBoards);
        memset(pieceList, ' ', SQUARE_NUMBER);
        std::vector<std::string> parsedPosition;
        SplitFenPosition(parsedPosition);
        ParseFenPiecePart(parsedPosition[0]);
        ParseSideToMovePart(parsedPosition[1]);
        ParseCastlingRightPart(parsedPosition[2]);
        enPassant = SquareToBitBoard(NotationToSquare(parsedPosition[3]));
        fiftyMoves = stoi(parsedPosition[4]);
        historyMovesNum = stoi(parsedPosition[5]);
        hashBoard = std::make_unique<Hash>(*this);
        ClearHistory();
    }

    void Board::ClearHistory()
    {
        if (!history.empty())
            history = std::stack<History>();
    }

    void Board::SplitFenPosition(std::vector<std::string> &fenParts)
    {
        boost::split(fenParts, fenPosition, boost::is_any_of(" "));
    }

    void Board::PrintBoard() const
    {
        //Timer t("print");
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
        auto enPassant_ = (enPassant != EMPTY_BOARD) ? SquareToNotation(TrailingZeroCount(enPassant)) : "-";
        std::cout << "En passant square: " << enPassant_ << std::endl;
        std::cout << "Castling rights: " << std::bitset<4>(static_cast<unsigned >(castlingRights)) << std::endl;
        std::cout << "Position key: " << std::hex << hashBoard->GetHash() << std::dec << std::endl;
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
                    pieceList[squareIndex] = fenChar;
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


    bool Board::MakeMove(Vixen::Move move)
    {
        //Timer<boost::chrono::nanoseconds> t("make");
        int from = move & 0x3F;
        int to = (move >> 6) & 0x3F;
        int moveType = move >> 12;
        int enPassantSquare = whiteToMove ? to - 8 : to + 8;
        char movingPieceLetter = GetPieceBoard(from);
        char capturedPieceLetter = GetPieceBoard(to);

        history.emplace(enPassant,
                        castlingRights,
                        fiftyMoves,
                        move,
                        movingPieceLetter,
                        capturedPieceLetter,
                        hashBoard->GetHash());

        ++fiftyMoves;

        if (enPassant != EMPTY_BOARD)
            hashBoard->HashEnPassant(enPassant);

        if (moveType == KING_CASTLE)
        {
            if (whiteToMove)
            {
                RemovePiece(H1, 'R');
                AddPiece(F1, 'R');
            }
            else
            {
                RemovePiece(H8, 'r');
                AddPiece(F8, 'r');
            }
        }

        else if (moveType == QUEEN_CASTLE)
        {
            if (whiteToMove)
            {
                RemovePiece(A1, 'R');
                AddPiece(D1, 'R');
            }
            else
            {
                RemovePiece(A8, 'r');
                AddPiece(D8, 'r');
            }
        }

        else if (moveType == ENPASSANT)
            whiteToMove ? RemovePiece(to - 8, 'p')
                        : RemovePiece(to + 8, 'P');

        else if (moveType & CAPTURE)
        {
            fiftyMoves = 0;
            RemovePiece(to, capturedPieceLetter);
        }

        else if (moveType & DOUBLE_PAWN_PUSH)
        {
            SetBit(enPassant, enPassantSquare);
            hashBoard->HashEnPassant(enPassant);
        }

        if (tolower(movingPieceLetter) == 'p')
            fiftyMoves = 0;

        RemovePiece(from, movingPieceLetter);

        if (moveType & PROMOTION)
        {
            std::string promotions = "nbrq";
            char promotion = promotions.at(static_cast<uint8_t>(moveType & 3));
            if (whiteToMove)
                promotion = static_cast<char>(toupper(promotion));
            AddPiece(to, promotion);
        }
        else
            AddPiece(to, movingPieceLetter);

        if (moveType == KING_CASTLE || moveType == QUEEN_CASTLE)
        {
            hashBoard->HashCastling(*this);
            castlingRights &= castlePermission[from];
            castlingRights &= castlePermission[to];
            hashBoard->HashCastling(*this);
        }

        whiteToMove = !whiteToMove;
        hashBoard->HashSide();
        ++historyMovesNum;

        if (whiteToMove ? MoveGenerator::IsInCheck<Colors::BLACK>(bitBoards, sliders)
                        : MoveGenerator::IsInCheck<Colors::WHITE>(bitBoards, sliders))
        {
            TakeBack();
            return false;
        }
        return true;
    }

    void Board::TakeBack()
    {
        if (history.empty())
            throw "Empty history";

        //Timer<boost::chrono::nanoseconds> t("take");
        History lastPosition = history.top();
        history.pop();

        int from = lastPosition.move & 0x3F;
        int to = (lastPosition.move >> 6) & 0x3F;
        int moveType = lastPosition.move >> 12;
        char movingPieceLetter = lastPosition.movedPiece;
        char capturedPieceLetter = lastPosition.capturedPiece;

        --historyMovesNum;
        whiteToMove = !whiteToMove;
        fiftyMoves = lastPosition.fiftyMoves;
        enPassant = lastPosition.enPassant;
        castlingRights = lastPosition.castlingRights;
        hashBoard->SetHash(lastPosition.hash);

        if (moveType & PROMOTION)
        {
            std::string promotions = "nbrq";
            char promotion = promotions.at(static_cast<uint8_t>(moveType & 3));
            if (whiteToMove)
                promotion = static_cast<char>(toupper(promotion));
            AddPiece(to, promotion);
        }

        else if (moveType == KING_CASTLE)
        {
            if (whiteToMove)
            {
                RemovePiece(F1, 'R');
                AddPiece(H1, 'R');
            }
            else
            {
                RemovePiece(F8, 'r');
                AddPiece(H8, 'r');
            }
        }

        else if (moveType == QUEEN_CASTLE)
        {
            if (whiteToMove)
            {
                RemovePiece(D1, 'R');
                AddPiece(A1, 'R');
            }
            else
            {
                RemovePiece(D8, 'r');
                AddPiece(A8, 'r');
            }
        }

        RemovePiece(to, movingPieceLetter);
        AddPiece(from, movingPieceLetter);

        if (moveType == ENPASSANT)
            whiteToMove ? AddPiece(to - 8, 'p')
                        : AddPiece(to + 8, 'P');

        else if (moveType & CAPTURE)
            AddPiece(to, capturedPieceLetter);


    }

    void Board::RemovePiece(int position, char pieceType)
    {
        pieceList[position] = ' ';
        ClearBit(bitBoards.at(pieceType), position);
        hashBoard->HashPiece(position, pieceType);
        islower(pieceType) ? ClearBit(bitBoards.at('S'), position) : ClearBit(bitBoards.at('F'), position);
        ToggleBit(bitBoards.at(' '), position);
    }

    void Board::AddPiece(int position, char pieceType)
    {
        pieceList[position] = pieceType;
        SetBit(bitBoards.at(pieceType), position);
        hashBoard->HashPiece(position, pieceType);
        islower(pieceType) ? SetBit(bitBoards.at('S'), position) : SetBit(bitBoards.at('F'), position);
        ToggleBit(bitBoards.at(' '), position);
    }

}